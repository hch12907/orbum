#pragma once
#include <cereal/cereal.hpp>
#include "Common/Types/Mips/BranchDelaySlot.hpp"
#include "Common/Types/Primitive.hpp"
#include "Common/Types/Register/PcRegisters.hpp"

/// BranchDelaySlot modified slightly for the VUs.
/// See BranchDelaySlot for more documentation.
class VuBranchDelaySlot : public BranchDelaySlot<1, Constants::EE::VPU::SIZE_VU_INSTRUCTION>
{
public:
    /// The VUs don't have the concept of virtual address, so just jump
    void set_branch_jtype(WordPcRegister& pc, const uptr jump_to)
    {
        add_branch_queue((jump_to * Constants::EE::VPU::SIZE_VU_INSTRUCTION) & 0x3FFF);
    }
};
