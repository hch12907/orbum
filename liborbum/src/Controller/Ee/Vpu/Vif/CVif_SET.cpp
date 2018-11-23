#include <boost/format.hpp>

#include "Core.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"

// Refer to EE Users Manual pg 104.
void CVif::STCYCL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // Writes CODE.IMMEDIATE to CYCLE
    const uword immediate = inst.imm();
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
    const uword immediate = inst.imm();
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
    const uword immediate = inst.imm();
    unit->base.insert_field(VifUnitRegister_Base::BASE, immediate);
}

// Refer to EE Users Manual pg 107.
void CVif::ITOP(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    const uword immediate = inst.imm();
    unit->itops.insert_field(VifUnitRegister_Itops::ITOPS, immediate);
}

// Refer to EE Users Manual pg 108.
void CVif::STMOD(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    const uword immediate = inst.imm();
    unit->mode.insert_field(VifUnitRegister_Mode::MOD, immediate);
}

void CVif::MSKPATH3(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    RResources& r = core->get_resources();

    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: MSKPATH3") % unit->core_id);
        return;
    }

    const uword immediate = inst.imm();
    const bool should_mask = immediate >> 15;

    r.ee.gif.stat.insert_field(GifRegister_Stat::M3P, should_mask);
}

void CVif::MARK(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    const uword immediate = inst.imm();
    unit->mark.insert_field(VifUnitRegister_Mark::MARK, immediate);
}

void CVif::STMASK(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    if (unit->subpackets_left)
        return;

    unit->mask.write_uword(unit->processing_data);
}

void CVif::STROW(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    if (unit->subpackets_left >= obtain_required_words(inst))
        return;

    SizedWordRegister *const row_regs[4] = { &unit->r3, &unit->r2, &unit->r1, &unit->r0 };

    row_regs[unit->subpackets_left - 1]->write_uword(unit->processing_data);
}

void CVif::STCOL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    if (unit->subpackets_left >= obtain_required_words(inst))
        return;

    SizedWordRegister *const col_regs[4] = { &unit->c3, &unit->c2, &unit->c1, &unit->c0 };

    col_regs[unit->subpackets_left - 1]->write_uword(unit->processing_data);
}
