#include <variant>

#include <boost/format.hpp>

#include "Controller/Ee/Vpu/Vu/Interpreter/CVuInterpreter.hpp"

#include "Core.hpp"
#include "Resources/RResources.hpp"

CVuInterpreter::CVuInterpreter(Core* core, int id) :
    CVu(core, id)
{
}

int CVuInterpreter::time_step(const int ticks_available)
{
#if defined(BUILD_DEBUG)
    DEBUG_LOOP_COUNTER++;
#endif

    RResources& r = core->get_resources();

    VuUnit_Base* const unit = r.ee.vpu.vu.units[core_id];

    // Move on if the unit is not running and the delay slot is empty
    if (unit->operation_state != VuOperationState::Run && !unit->bdelay.is_branch_pending())
    {
        return 1;
    }

    // PC & Instructions stuff...
    const uword pc = unit->pc.read_uword() & 0x0FFF;
    const udword raw_inst = unit->micro_mem->read_uword(pc);

    const uword upper_raw_inst = (raw_inst >> 32) & 0xFFFFFFFF;
    const VuInstruction upper_inst = VuInstruction(upper_raw_inst);
    const MipsInstructionInfo upper_info = upper_inst.get_upper_info();
    const VuInstructionDecoder upper_decoder = VuInstructionDecoder(upper_inst, upper_info);

    const uword lower_raw_inst = raw_inst & 0xFFFFFFFF;
    const VuInstruction lower_inst = VuInstruction(lower_raw_inst);
    const MipsInstructionInfo lower_info = lower_inst.get_lower_info();
    const VuInstructionDecoder lower_decoder = VuInstructionDecoder(lower_inst, lower_info);

    // Flush the pipelines
    unit->efu.consume_cycle(1);
    unit->fdiv.consume_cycle(1);
    unit->ialu.consume_cycle(1);
    unit->lsu.consume_cycle(1);

    for (FmacPipeline& fmac : unit->fmac)
    {
        fmac.consume_cycle(1);
    }

    // If the units have finished execution, replace the original regs with new ones
    if (!unit->efu.is_running())
        unit->p = unit->efu.new_p;
    if (!unit->fdiv.is_running())
        unit->q = unit->fdiv.new_q;

    bool data_hazard_occured = check_data_hazard(unit, upper_decoder, lower_decoder);

    // If I (bit 63) is set, execute UpperInst and LOI (using LowerInst as an immediate)
    if ((raw_inst >> 63) & 1)
    {
        execute_upper_instruction(unit, upper_decoder, data_hazard_occured);
        this->LOI(unit, lower_inst);

        // Advance PC and onto the next cycle
        unit->bdelay.advance_pc(unit->pc);
        return 1;
    }

    // If E (bit 62) is set, execute current and next instruction, and
    // terminate the current micro subroutine
    // Here we setup a delay slot for the next instruction
    if ((raw_inst >> 62) & 1)
    {
        if (unit->bdelay.is_branch_pending())
        {
            BOOST_LOG(Core::get_logger()) << "Found E-bit in branch delay slot";
        }

        // Don't actually branch
        unit->bdelay.set_branch_itype(unit->pc, 0);

        // Change the state of the VU
        unit->operation_state = VuOperationState::Ready;
    }

    // If M (bit 61) is set, then execute QMTC2 or CTC2 without interlocking
    // (VU0 only)
    if (((raw_inst >> 61) & 1) && unit->core_id == 0)
    {
        VuUnit_Vu0& vu = r.ee.vpu.vu.unit_0;
        if (vu.transferred_reg.has_value())
        {
            *vu.ccr[vu.transferred_reg_location] = vu.transferred_reg.value();
            vu.transferred_reg = std::nullopt;
        }
    }

    // If D (bit 60) and DE (in FBRST) is set, terminate the micro subroutine and interrupt
    if ((raw_inst >> 60) & 1)
    {
        if (r.ee.vpu.vu.fbrst.de(unit->core_id))
        {
            auto _lock = r.ee.intc.stat.scope_lock();
            r.ee.intc.stat.insert_field(EeIntcRegister_Stat::VU_KEYS[unit->core_id], 1);
            unit->operation_state = VuOperationState::Stop;
        }
    }

    // If T (bit 59) and TE (in FBRST) is set, terminate the micro subroutine and interrupt
    if ((raw_inst >> 59) & 1)
    {
        if (r.ee.vpu.vu.fbrst.te(unit->core_id))
        {
            auto _lock = r.ee.intc.stat.scope_lock();
            r.ee.intc.stat.insert_field(EeIntcRegister_Stat::VU_KEYS[unit->core_id], 1);
            unit->operation_state = VuOperationState::Stop;
        }
    }

    // Register writing priority, if both upper and lower inst write to the same
    // register, the priority is: COP2 Transfer > Upper Inst > Lower Inst
    try
    {
        // Try obtaining the destination (will throw if the instruction writes to non-VF/VI regs)
        const uword upper_dest = *upper_decoder.try_get_dest();
        const uword lower_dest = *lower_decoder.try_get_dest();

        // Check if the lower instruction write to VI or VF
        // If it writes to VF, check if it writes to the same reg
        if (!lower_decoder.is_integer_instruction())
        {
            if (upper_dest == lower_dest)
            {
                SizedQwordRegister original_vf = unit->vf[upper_dest];
                execute_lower_instruction(unit, lower_decoder, data_hazard_occured);

                // The result produced by lower instruction is discarded
                unit->vf[upper_dest] = original_vf;
                execute_upper_instruction(unit, upper_decoder, data_hazard_occured);
            }
        }
        else
        {
            // Otherwise just run it as usual
            execute_upper_instruction(unit, upper_decoder, data_hazard_occured);
            execute_lower_instruction(unit, lower_decoder, data_hazard_occured);
        }
    }
    catch (std::exception)
    {
        // If one of them write to special regs (P, Q, etc), execute like usual
        execute_upper_instruction(unit, upper_decoder, data_hazard_occured);
        execute_lower_instruction(unit, lower_decoder, data_hazard_occured);
    }

    // Advance the PC
    if (!check_data_hazard(unit, upper_decoder, lower_decoder))
        unit->bdelay.advance_pc(unit->pc);

    // TODO: Correct CPI
    return 1;
}

int CVuInterpreter::execute_lower_instruction(VuUnit_Base* unit, const VuInstructionDecoder& decoder, bool data_hazard)
{
    const VuInstruction& inst = decoder.instruction;
    const MipsInstructionInfo& info = decoder.instruction_info;

    // If there's a data hazard, stall
    if (data_hazard)
        return 1;

    switch (info.pipeline)
    {
    case VuPipeline::EFU:
    {
        if (!unit->efu.is_running())
        {
            // The 3 cycles: Fetch, obtain VPU register, write-back. As the
            // EFU handles only the execution stage, other cycles are ignored
            unit->efu = EfuPipeline(info.cpi - 3);

            // Store original unit->p in new_p temporarily before executing the
            // instruction (since it operates on unit->p) and swap it back later.
            std::swap(unit->p, unit->efu.new_p);
            (this->*VU_INSTRUCTION_TABLE[info.impl_index])(unit, inst);
            std::swap(unit->p, unit->efu.new_p);
        }

        break;
    }

    case VuPipeline::FDIV:
    {
        if (!unit->fdiv.is_running())
        {
            // The 2 cycles: Fetch, obtain VPU register. As the FDIV handles
            // both execution & write-back stage, other cycles are ignored
            unit->fdiv = FdivPipeline(info.cpi - 2);

            // See EFU.
            std::swap(unit->q, unit->fdiv.new_q);
            (this->*VU_INSTRUCTION_TABLE[info.impl_index])(unit, inst);
            std::swap(unit->q, unit->fdiv.new_q);
        }

        break;
    }

    case VuPipeline::IALU:
    {
        if (!unit->ialu.is_running())
        {
            // Try to get the destination
            int dest = decoder.try_get_dest().value_or(0);

            // See FDIV
            unit->ialu = IaluPipeline(info.cpi - 2, dest);
        }

        // The results are bypassed to other instructions directly, so
        // there's no need to swap stuff around
        (this->*VU_INSTRUCTION_TABLE[info.impl_index])(unit, inst);

        break;
    }

    case VuPipeline::LSU:
    {
        if (!unit->lsu.is_running())
        {
            int dest = decoder.try_get_dest().value_or(0);

            unit->lsu = LsuPipeline(info.cpi - 2, dest);
        }

        break;
    }

    case VuPipeline::Basic:
    {
        (this->*VU_INSTRUCTION_TABLE[info.impl_index])(unit, inst);
    }
    }

    return 1;
}

int CVuInterpreter::execute_upper_instruction(VuUnit_Base* unit, const VuInstructionDecoder& decoder, bool data_hazard)
{
    const VuInstruction& inst = decoder.instruction;
    const MipsInstructionInfo& info = decoder.instruction_info;

    // Stall if there's a data hazard
    if (data_hazard)
        return 1;

    for (FmacPipeline& fmac : unit->fmac)
    {
        if (!fmac.is_running())
        {
            fmac = FmacPipeline(info.cpi - 2, decoder.try_get_dest().value_or(0), inst.dest());
            (this->*VU_INSTRUCTION_TABLE[info.impl_index])(unit, inst);
            break;
        }
    }

    return 1;
}

bool CVuInterpreter::check_data_hazard(VuUnit_Base* unit, const VuInstructionDecoder& ud, const VuInstructionDecoder& ld) const
{
    // Obtain the registers to be read by the instruction
    // If the instruction does not specify the field(s), use 0 as placeholder,
    // as VF0/VI0 is hardwired to 0 (and the manual did this too)
    const int upper_read[3] = {
        ud.try_get_src(0).value_or(0),
        ud.try_get_src(1).value_or(0),
        ud.try_get_src(2).value_or(-1) // this one's special because only MADD/MSUB uses this
    };

    const int lower_read[2] = {
        ld.try_get_src(0).value_or(0),
        ld.try_get_src(1).value_or(0)
    };

    // If the instruction is WAITP, return true if EFU is running
    if (((ld.instruction.value) & 0x7FF) == 0x7BF)
    {
        if (unit->efu.is_running())
            return true;
    }

    // If the instruction is WAITQ, return true if FDIV is running
    if (((ld.instruction.value) & 0x7FF) == 0x3BF)
    {
        if (unit->fdiv.is_running())
            return true;
    }

    // Upper Instructions data hazard check
    {
        const VuInstruction inst = ud.instruction;

        if (upper_read[2] == -1)
        {
            for (FmacPipeline& fmac : unit->fmac)
            {
                if (fmac.is_using_register(upper_read[0], *ud.try_get_src_field(0)))
                    return true;
                if (fmac.is_using_register(upper_read[1], *ud.try_get_src_field(1)))
                    return true;
            }
        }
        else
        {
            for (FmacPipeline& fmac : unit->fmac)
            {
                if (fmac.is_using_register(upper_read[0], *ud.try_get_src_field(0)))
                    return true;
                if (fmac.is_using_register(upper_read[1], *ud.try_get_src_field(1)))
                    return true;
                if (fmac.is_using_register(upper_read[2], *ud.try_get_src_field(2)))
                    return true;
            }
        }
    }

    // Lower Instructions data hazard check
    {
        const VuInstruction inst = ld.instruction;

        for (FmacPipeline& fmac : unit->fmac)
        {
            if (fmac.is_using_register(lower_read[0], *ld.try_get_src_field(0)))
                return true;
            if (fmac.is_using_register(lower_read[1], *ld.try_get_src_field(1)))
                return true;
        }    
    }

    return false;
}
