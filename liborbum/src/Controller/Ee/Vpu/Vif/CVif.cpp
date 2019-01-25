#include <boost/format.hpp>

#include "Controller/Ee/Vpu/Vif/CVif.hpp"

#include "Core.hpp"

CVif::CVif(Core* core, int id) :
    CController(core),
    core_id(id)
{
}

void CVif::handle_event(const ControllerEvent& event)
{
    switch (event.type)
    {
    case ControllerEvent::Type::Time:
    {
        int ticks_remaining = time_to_ticks(event.data.time_us);
        while (ticks_remaining > 0)
            ticks_remaining -= time_step(ticks_remaining);
        break;
    }
    default:
    {
        throw std::runtime_error("CVif event handler not implemented - please fix!");
    }
    }
}

int CVif::time_to_ticks(const double time_us)
{
    int ticks = static_cast<int>(time_us / 1.0e6 * Constants::EE::EEBUS_CLK_SPEED * core->get_options().system_bias_vif);

    if (ticks < 10)
    {
        static bool warned = false;
        if (!warned)
        {
            BOOST_LOG(Core::get_logger()) << "Vif ticks too low - increase time delta";
            warned = true;
        }
    }

    return ticks;
}

int CVif::time_step(const int ticks_available)
{
    auto& r = core->get_resources();

    VifUnit_Base* unit = r.ee.vpu.vif.units[core_id];
    // If STC is written, reset the following fields of VIF.STAT:
    // VSS, VFS, VIS, INT, ER0, ER1
    if (unit->fbrst.extract_field(VifUnitRegister_Fbrst::STC))
    {
        uword stat = unit->stat.read_uword();
        stat = VifUnitRegister_Stat::VSS.insert_into(stat, 0u);
        stat = VifUnitRegister_Stat::VFS.insert_into(stat, 0u);
        stat = VifUnitRegister_Stat::VIS.insert_into(stat, 0u);
        stat = VifUnitRegister_Stat::INT.insert_into(stat, 0u);
        stat = VifUnitRegister_Stat::ER0.insert_into(stat, 0u);
        stat = VifUnitRegister_Stat::ER1.insert_into(stat, 0u);

        unit->stat.write_uword(stat);
    }

    // Check if VIF is stalled, do not do anything (FBRST.STC needs to be written to before we return).
    if (unit->stat.is_stalled())
        return 1;

    // If the VIF is waiting for the VU, run the current instruction and call it a day
    if (unit->stat.extract_field(VifUnitRegister_Stat::VEW))
    {
        (this->*INSTRUCTION_TABLE[unit->inst->get_info()->impl_index])(unit, *unit->inst);
        return 1;
    }

    // Check the FIFO queue for incoming DMA packet. Exit early if there is nothing to process.
    if (!unit->dma_fifo_queue->has_read_available(NUMBER_BYTES_IN_QWORD))
        return 1;

    // Four VIF statuses: 0b00 Idle, 01 Waiting for data, 10 Decoding VIFcode, 11 Decompressing data
    const ubyte status = unit->stat.extract_field(VifUnitRegister_Stat::VPS);

    // Check for STP
    if (!status && unit->fbrst.extract_field(VifUnitRegister_Fbrst::STP))
    {
        unit->stat.insert_field(VifUnitRegister_Stat::VSS, 1);
        unit->fbrst.insert_field(VifUnitRegister_Fbrst::STP, 0);
        return 1;
    }

    // If RST is written, reset the VIF
    if (!status && unit->fbrst.extract_field(VifUnitRegister_Fbrst::RST))
    {
        BOOST_LOG(Core::get_logger()) << "VIF: Resetting VIF" << unit->core_id;
        unit->fbrst.insert_field(VifUnitRegister_Fbrst::RST, 0);
        
        // Reinitialize the VIF unit
        unit->dma_fifo_queue->initialize();
        *unit = VifUnit_Base(unit->core_id, unit->dma_fifo_queue);

        return 1;
    }

    for (int i = unit->packet_progress; i < NUMBER_WORDS_IN_QWORD; i++, unit->packet_progress = i)
    {
        unit->dma_fifo_queue->read(reinterpret_cast<ubyte*>(&unit->processing_data), NUMBER_BYTES_IN_WORD);

        const uword& data = unit->processing_data;

        // If the VIF is idling, treat the data as a VIFcode
        if (!status)
        {
            unit->code.write_uword(data);
            unit->inst = std::make_unique<VifcodeInstruction>(data);
            unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b10);
            unit->subpackets_left = obtain_required_words(*unit, *unit->inst);
            BOOST_LOG(Core::get_logger()) << "VIF: Fetched instruction " << unit->inst->get_info()->mnemonic;
            return 1;
        }
        else
        {
            // If there are no packets left, set the VIF status to idle
            if (!unit->subpackets_left)
            {
                unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b00);
                // If the VIF is idling, treat the data as a VIFcode
                if (!status)
                {
                    unit->code.write_uword(data);
                    unit->inst = std::make_unique<VifcodeInstruction>(data);
                    unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b10);
                    unit->subpackets_left = obtain_required_words(*unit, *unit->inst);
                    
                    // If the VIFcode is valid, set ER1 to 0
                    if (!unit->inst->get_info()->impl_index)
                    {
                        unit->stat.insert_field(VifUnitRegister_Stat::ER1, 0);
                    }

                    BOOST_LOG(Core::get_logger()) << "VIF: Fetched instruction " << unit->inst->get_info()->mnemonic;
                    
                    // TODO: confirm behaviour
                    // Does it take one whole cycle for the VIF to process a VIFcode?
                    return 1;
                }
            }
            else
            {
                unit->subpackets_left--;
                
                // If there are no packets left, set the VIF status to idle
                if (!unit->subpackets_left)
                {
                    unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b00);

                    // If the I bit is set, raise an interrupt
                    if (unit->inst->i() && !unit->err.extract_field(VifUnitRegister_Err::MII))
                    {
                        unit->stat.insert_field(VifUnitRegister_Stat::INT, 1);
                        auto _lock = r.ee.intc.stat.scope_lock();
                        r.ee.intc.stat.insert_field(EeIntcRegister_Stat::VIF_KEYS[unit->core_id], 1);
                    }

                    return 1;
                }

                unit->subpackets_left--;

                (this->*INSTRUCTION_TABLE[unit->inst->get_info()->impl_index])(unit, *unit->inst);
            }
        }
    }

    if (unit->packet_progress >= NUMBER_WORDS_IN_QWORD)
    {
        unit->packet_progress = 0;
    }

    return 1;
}

int CVif::obtain_required_words(const VifUnit_Base& unit, const VifcodeInstruction inst) const
{
    switch (inst.get_info()->cpi)
    {
    case SpecialVifcodePacketUsage::Num:
        // branchless version of `1 + inst.num() ? inst.num() : 256` (0 = 2^8)
        return 1 + (((inst.num() - 1) & 0xFF) + 1) * 2;

    case SpecialVifcodePacketUsage::Immediate:
        // branchless version of `1 + inst.imm() ? inst.imm() : 65536` (0 = 2^16)
        return 1 + (((inst.imm() - 1) & 0xFFFF) + 1) * 4;

    case SpecialVifcodePacketUsage::Unpack:
    {
        const int cl = unit.cycle.extract_field(VifUnitRegister_Cycle::CL);
        const int wl = unit.cycle.extract_field(VifUnitRegister_Cycle::WL);

        // The length of each element, in bits
        const int element_length = 32 >> inst.vl();
        // The number of elements in each data
        const int num_of_element = inst.vn() + 1;

        if (wl > cl)
        {
            // The number of data
            const int data_num = cl * (inst.num() / wl) + std::min(inst.num() % wl, cl);
            // The length of the data in bits
            const int data_length = element_length * num_of_element * data_num;
            // The length of the data in words (quotient is rounded up if there are remainders, hence the +31)
            const int data_words = (data_length + 31) >> 5;

            return 1 + data_words;
        }
        else
        {
            // The length of the data in bits
            const int data_length = element_length * num_of_element * inst.num();
            // The length of the data in words (quotient is rounded up if there are remainders, hence the +31)
            const int data_words = (data_length + 31) >> 5;

            return 1 + data_words;
        }
    }

    default:
        return inst.get_info()->cpi;
    }
}

void CVif::INSTRUCTION_UNSUPPORTED(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    if (unit->err.extract_field(VifUnitRegister_Err::ME1))
    {
        return NOP(unit, inst);
    }

    unit->stat.insert_field(VifUnitRegister_Stat::ER1, 1);

    throw std::runtime_error("VIFcode CMD field was invalid! Please fix.");
}

// Refer to EE Users Manual pg 103.
void CVif::NOP(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // nothing to do
    return;
}

template <typename T>
void CVif::write_vu_mem(VifUnit_Base* unit, const uword offset, const T data)
{
    RResources& r = core->get_resources();
    ArrayByteMemory& vu_mem = *r.ee.vpu.vu.units[unit->core_id]->vu_mem;

    const uword vu_addr = unit->inst->addr();

}

// Implement write_vu_mem(..) only for selected types 
// Note: the requirement for T is 4 bytes long
template void CVif::write_vu_mem<uword>(VifUnit_Base* unit, const uword offset, const uword data);
template void CVif::write_vu_mem<sword>(VifUnit_Base* unit, const uword offset, const sword data);
template void CVif::write_vu_mem< f32 >(VifUnit_Base* unit, const uword offset, const f32   data);
