#include <boost/format.hpp>

#include "Core.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"

void CVif::MPG(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    RResources& r = core->get_resources();
    
    VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];
    ArrayByteMemory* vu_micro_mem[2] = { &r.ee.vpu.vu.unit_0.memory_micro, &r.ee.vpu.vu.unit_1.memory_micro };

    const bool vu_is_running = vu->operation_state == VuOperationState::Run;
    
    if (vu_is_running)
    {
        unit->stat.insert_field(VifUnitRegister_Stat::VEW, 1);
        return;
    }

    // First pass - decoding VIF code & obtaining the num
    if (unit->subpackets_left == obtain_required_words(inst))
    {
        unit->num.write_uword(inst.num());
        unit->stat.insert_field(VifUnitRegister_Stat::VFS, 0b11);
    }

    // Other passes - transfer data to VU mem
    if (unit->subpackets_left < obtain_required_words(inst))
    {
        const uword starting_addr = inst.imm() * 8;
        const uword offset = (inst.num() - unit->num.read_uword()) * 4 + unit->packet_progress;

        vu_micro_mem[unit->core_id]->write_uword(starting_addr + offset, unit->processing_data);

        unit->num.write_uword(unit->num.read_uword() - 1);
    }
}

void CVif::DIRECT(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: DIRECT") % unit->core_id);
        return;
    }

    // TODO: Implement this
}

void CVif::DIRECTHL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: DIRECTHL") % unit->core_id);
        return;
    }

    // TODO: Implement this
}
