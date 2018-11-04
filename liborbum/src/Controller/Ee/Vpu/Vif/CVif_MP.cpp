#include <boost/format.hpp>

#include "Core.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"

void CVif::FLUSHE(VifUnit_Base* unit, const VifcodeInstruction inst)
{
}

void CVif::FLUSH(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: FLUSH") % unit->core_id);
        return;
    }

    // TODO: Implement this
}

void CVif::FLUSHA(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: FLUSHA") % unit->core_id);
        return;
    }

    // TODO: Implement this
}

void CVif::MSCAL(VifUnit_Base* unit, const VifcodeInstruction inst)
{
}

void CVif::MSCNT(VifUnit_Base* unit, const VifcodeInstruction inst)
{
}

void CVif::MSCALF(VifUnit_Base* unit, const VifcodeInstruction inst)
{
    // VIF1 only
    if (unit->core_id != 1)
    {
        BOOST_LOG(Core::get_logger()) << str(boost::format("Warning: VIF%d called a VIF1-only instruction: MSCALF") % unit->core_id);
        return;
    }

    // TODO: Implement this
}