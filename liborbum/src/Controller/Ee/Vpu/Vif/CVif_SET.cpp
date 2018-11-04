#include <boost/format.hpp>

#include "Core.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"

// Refer to EE Users Manual pg 104.
void CVif::STCYCL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // Writes CODE.IMMEDIATE to CYCLE
    uword immediate = inst.imm();
    unit->cycle.write_uword(immediate);
}

// Refer to EE Users Manual pg 105.
void CVif::OFFSET(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: OFFSET") % unit->core_id);
        return;
    }

    // Clear STAT.DBF
    unit->stat.insert_field(VifUnitRegister_Stat::DBF, 0);

    // Writes the lower 10 bits of CODE.IMMEDIATE to OFST
    uword immediate = inst.imm();
    unit->ofst.insert_field(VifUnitRegister_Ofst::OFFSET, immediate);

    // Transfer BASE to TOPS
    unit->tops.write_uword(unit->base.read_uword());
}

// Refer to EE Users Manual pg 106.
void CVif::BASE(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: BASE") % unit->core_id);
        return;
    }

    // Writes the lower 10 bits of CODE.IMMEDIATE to BASE
    uword immediate = inst.imm();
    unit->base.insert_field(VifUnitRegister_Base::BASE, immediate);
}

// Refer to EE Users Manual pg 107.
void CVif::ITOP(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    uword immediate = inst.imm();
    unit->itops.insert_field(VifUnitRegister_Itops::ITOPS, immediate);
}

// Refer to EE Users Manual pg 108.
void CVif::STMOD(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    uword immediate = inst.imm();
    unit->mode.insert_field(VifUnitRegister_Mode::MOD, immediate);
}

void CVif::MSKPATH3(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: MSKPATH3") % unit->core_id);
        return;
    }

    // TODO: Implement this when GIF is implemented
}

void CVif::MARK(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    uword immediate = inst.imm();
    unit->mark.insert_field(VifUnitRegister_Mark::MARK, immediate);
}

void CVif::STMASK(VifUnit_Base* unit, const VifcodeInstruction inst)
{
}

void CVif::STROW(VifUnit_Base* unit, const VifcodeInstruction inst)
{
}

void CVif::STCOL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
}
