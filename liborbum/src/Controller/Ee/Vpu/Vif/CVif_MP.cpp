#include <boost/format.hpp>

#include "Core.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"

void CVif::FLUSHE(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    const RResources& r = core->get_resources();
    const VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];
    unit->stat.insert_field(VifUnitRegister_Stat::VEW, vu->operation_state == VuOperationState::Run);
}

void CVif::FLUSH(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: FLUSH") % unit->core_id);
        return;
    }

    const RResources& r = core->get_resources();
    const VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];

    // TODO: Confirm behaviour.
    const bool condition = vu->operation_state == VuOperationState::Run
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P1Q) 
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P2Q);
        
    unit->stat.insert_field(VifUnitRegister_Stat::VEW, condition);
}

void CVif::FLUSHA(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: FLUSHA") % unit->core_id);
        return;
    }

    const RResources& r = core->get_resources();
    const VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];

    const bool condition = vu->operation_state == VuOperationState::Run
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P1Q) 
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P2Q)
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P3Q);
        
    unit->stat.insert_field(VifUnitRegister_Stat::VEW, condition);
}

void CVif::MSCAL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    BOOST_LOG(Core::get_logger()) << "MSCAL called - dis gon b gud";

    RResources& r = core->get_resources();
    
    VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];

    const bool vu_is_running = vu->operation_state == VuOperationState::Run;
    
    if (vu_is_running)
    {
        unit->stat.insert_field(VifUnitRegister_Stat::VEW, 1);
        return;
    }

    // Set the VU state and its PC
    vu->operation_state = VuOperationState::Run;
    vu->pc.write_uword(inst.imm() * 8);
}

void CVif::MSCNT(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    BOOST_LOG(Core::get_logger()) << "MSCNT called - dis gon b gud";

    RResources& r = core->get_resources();
    
    VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];

    const bool vu_is_running = vu->operation_state == VuOperationState::Run;
    
    if (vu_is_running)
    {
        unit->stat.insert_field(VifUnitRegister_Stat::VEW, 1);
        return;
    }

    // Set the VU state
    vu->operation_state = VuOperationState::Run;
}

void CVif::MSCALF(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    BOOST_LOG(Core::get_logger()) << "MSCALF called - dis gon b gud";

    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: MSCALF") % unit->core_id);
        return;
    }

    RResources& r = core->get_resources();
    
    VuUnit_Base *const vu = r.ee.vpu.vu.units[unit->core_id];

    const bool should_wait = vu->operation_state == VuOperationState::Run
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P1Q) 
        || r.ee.gif.stat.extract_field(GifRegister_Stat::P2Q);
    
    if (should_wait)
    {
        unit->stat.insert_field(VifUnitRegister_Stat::VEW, 1);
        return;
    }

    // Set the VU state
    vu->operation_state = VuOperationState::Run;
    vu->pc.write_uword(inst.imm() * 8);
}
