#include <boost/format.hpp>

#include "Controller/Ee/Vpu/Vif/CVif.hpp"

#include "Core.hpp"

CVif::CVif(Core* core) :
    CController(core)
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

    for (auto& unit : r.ee.vpu.vif.units)
    {
        // Check if VIF is stalled, do not do anything (FBRST.STC needs to be written to before we continue).
        if (unit->stat.is_stalled())
            continue;

        // If the VIF is waiting for the VU, run the current instruction and call it a day
        if (unit->stat.extract_field(VifUnitRegister_Stat::VEW))
        {
            (this->*INSTRUCTION_TABLE[unit->inst->get_info()->impl_index])(unit, *unit->inst);
            continue;
        }

        // Check the FIFO queue for incoming DMA packet. Exit early if there is nothing to process.
        if (!unit->dma_fifo_queue->has_read_available(NUMBER_BYTES_IN_QWORD))
            continue;

        for (int i = unit->packet_progress; i < NUMBER_WORDS_IN_QWORD; i++, unit->packet_progress = i)
        {
            unit->dma_fifo_queue->read(reinterpret_cast<ubyte*>(&unit->processing_data), NUMBER_BYTES_IN_WORD);

            // Four VIF statuses: 0b00 Idle, 01 Waiting for data, 10 Decoding VIFcode, 11 Decompressing data
            const ubyte status = unit->stat.extract_field(VifUnitRegister_Stat::VPS);
            
            const uword& data = unit->processing_data;

            // If the VIF is idling, treat the data as a VIFcode
            if (!status)
            {
                unit->code.write_uword(data);
                unit->inst = std::make_unique<VifcodeInstruction>(data);
                unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b10);
                unit->subpackets_left = obtain_required_words(*unit, *unit->inst);
                BOOST_LOG(Core::get_logger()) << "VIF: Fetched instruction " << unit->inst->get_info()->mnemonic;
                continue;
            }
            else
            {
                // If there are no packets left, set the VIF status to idle
                if (!unit->subpackets_left)
                {
                    unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b00);

                    // If the I bit is set, raise an interrupt
                    if ((*unit->inst).i())
                    {
                        auto _lock = r.ee.intc.stat.scope_lock();
                        r.ee.intc.stat.insert_field(EeIntcRegister_Stat::VIF_KEYS[unit->core_id], 1);
                    }

                    continue;
                }

                unit->subpackets_left--;

                (this->*INSTRUCTION_TABLE[unit->inst->get_info()->impl_index])(unit, *unit->inst);
            }
        }

        if (unit->packet_progress >= NUMBER_WORDS_IN_QWORD)
        {
            unit->packet_progress = 0;
        }
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
        
    default:
        return inst.get_info()->cpi;
    }
}

void CVif::INSTRUCTION_UNSUPPORTED(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    throw std::runtime_error("VIFcode CMD field was invalid! Please fix.");
}

// Refer to EE Users Manual pg 103.
void CVif::NOP(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // nothing to do
    return;
}
