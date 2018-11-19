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

        // Check the FIFO queue for incoming DMA packet. Exit early if there is nothing to process.
        if (!unit->dma_fifo_queue->has_read_available(NUMBER_BYTES_IN_QWORD))
            continue;

        uqword raw_data;
        unit->dma_fifo_queue->read(reinterpret_cast<ubyte*>(&raw_data), NUMBER_BYTES_IN_QWORD);

        for (uword data : raw_data.uw)
        {
            // Four VIF statuses: 0b00 Idle, 01 Waiting for data, 10 Decoding VIFcode, 11 Decompressing data
            const ubyte status = unit->stat.extract_field(VifUnitRegister_Stat::VPS);
            
            unit->processing_data = data;

            // If the VIF is idling, treat the data as a VIFcode
            if (!status)
            {
                unit->code.write_uword(data);
                unit->inst = std::make_unique<VifcodeInstruction>(data);
                unit->stat.insert_field(VifUnitRegister_Stat::VPS, 0b10);
                unit->packets_left = obtain_required_words(*unit->inst);

                continue;
            }
            else
            {
                unit->packets_left--;

                (this->*INSTRUCTION_TABLE[unit->inst->get_info()->impl_index])(unit, *unit->inst);

                // If there are no packets left, set the VIF status to idle
                if (!unit->packets_left)
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
            }
        }
    }

    return 1;
}

int CVif::obtain_required_words(const VifcodeInstruction instruction) const
{
    switch (instruction.get_info()->cpi)
    {
    case SpecialVifcodePacketUsage::Num:
        return 1 + instruction.num() * 2;

    case SpecialVifcodePacketUsage::Immediate:
        return 1 + instruction.imm() * 4;

    default:
        return instruction.get_info()->cpi;
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
