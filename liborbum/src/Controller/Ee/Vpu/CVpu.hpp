#pragma once

#include "Controller/CController.hpp"
#include "Controller/ControllerEvent.hpp"
#include "Controller/Ee/Vpu/Vif/CVif.hpp"
#include "Controller/Ee/Vpu/Vu/CVu.hpp"
#include "Core.hpp"

class Core;

template<typename VuController>
class CVpu : public CController
{
public:
    CVpu(Core* core, int id) :
        CController(core),
        core_id(id),
        vif(CVif(core, id)),
        vu(VuController(core, id))
    {
    }

    void handle_event(const ControllerEvent& event)
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
            throw std::runtime_error("CVpu event handler not implemented - please fix!");
        }
        }
    }

    /// Runs the VU and VIF.
    int time_step(const int ticks_available)
    {
        // Run VIF and VU for one cycle.
        vif.time_step(1);
        vu.time_step(1);
        return 1;
    }

private:
    /// Converts a time duration into the number of ticks that would have occurred.
    int time_to_ticks(const double time_us)
    {
        int ticks = static_cast<int>(time_us / 1.0e6 * Constants::EE::VPU::VU::VU_CLK_SPEED * core->get_options().system_bias_vu);

        if (ticks < 10)
        {
            static bool warned = false;
            if (!warned)
            {
                BOOST_LOG(Core::get_logger()) << "VPU ticks too low - increase time delta";
                warned = true;
            }
        }

        return ticks;
    }

    /// The ID of the VPU.
    int core_id;

    /// VU/VIF executors (can be recompiler or interpreter)
    CVif vif;
    VuController vu;
};
