#include <boost/format.hpp>

#include "Core.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"

void CVif::MPG(VifUnit_Base* unit, const VifcodeInstruction inst)
{
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
