#pragma once

#include <array>
#include <utility>

#include <cereal/cereal.hpp>

#include "Common/Types/Primitive.hpp"
#include "Common/Types/Register/PcRegisters.hpp"

/// Contains functionallity for the branch delay slot featured in MIPS CPU's.
/// A branch delay occurs when a jump/branch instruction is hit,
/// and the subsequent instructions are known as the "branch slots".
/// These instructions in the branch slots are executed until the number of
/// configured branch slots have been run, and then the jump/branch occurs.
/// A default slot size of 1 is used by the EE and IOP cores.
/// slot + 1 is used internally as this is the real amount of instruction cycles.
/// inst_size specifies the instruction size of the unit (4 bytes for EE & IOP, 8 for VU)
template <size_t slots = 1, size_t inst_size = Constants::MIPS::SIZE_MIPS_INSTRUCTION>
class BranchDelaySlot
{
public:
    BranchDelaySlot() :
        pending_branches { std::pair(-1, 0), std::pair(-1, 0) }
    {
    }

    void add_branch_queue(const uptr address)
    {
        for (auto& branch : pending_branches)
        {
            if (branch.first == -1)
            {
                branch.first = slots + 1;
                branch.second = address;
                
                return;
            }
        }

        throw std::runtime_error("BDS: More than 2 branches in queue - this should not happen");
    }

    /// Obtains the PC of a soon-to-happen branching.
    /// This is used by the *AL* (link register) branching/jumping instructions.
    uptr get_branch_pc()
    {
        return pending_branches[0].second;
    }

    /// Sets a pending branch to the direct address given.
    void set_branch_direct(const uptr address)
    {
        add_branch_queue(address);
    }

    /// Sets a pending branch which combines the current PC address with
    /// the offset specified. Used for I-type instructions (imm's).
    void set_branch_itype(WordPcRegister& pc, const shword imm)
    {
        add_branch_queue((pc.read_uword() + inst_size) + (imm * inst_size));
    }

    /// Sets a pending branch which combines the current PC address with
    /// the region address. Used for J-type instructions.
    void set_branch_jtype(WordPcRegister& pc, const uptr j_region_addr)
    {
        add_branch_queue(((pc.read_uword() + inst_size) & 0xF0000000) | (j_region_addr * inst_size));
    }

    /// Advances the PC by either incrementing by 1 instruction,
    /// or taking a branch if all slots have been used.
    void advance_pc(WordPcRegister& pc)
    {
        for (auto& branch : pending_branches)
        {
            // Run for one cycle if >= 0
            // (-1 is used for indicating empty branch, and 0 for "time to branch")
            if (branch.first >= 0)
                branch.first -= 1;
        }

        // Check if the cycles needed to branch reaches 0.
        if (pending_branches[0].first == 0)
        {
            pc.write_uword(pending_branches[0].second);

            // Move second branch up the queue
            pending_branches[0].first = pending_branches[1].first;
            pending_branches[0].second = pending_branches[1].second;

            // Set second branch to empty
            pending_branches[0].first = -1;
            pending_branches[0].second = 0;

            return;
        }

        // Increment by 1 instruction.
        pc.offset(inst_size);
    }

    /// Stops the current branch in progress (used by exception handler).
    void stop_branch()
    {
        for (auto& branch : pending_branches)
        {
            branch.first = -1;
        }
    }

    /// Returns if a branch is currently pending.
    bool is_branch_pending() const
    {
        // Since pending_branches[1] will be moved to [0], just checking [0] would be enough
        return pending_branches[0].first != -1;
    }

protected:
    // sword of std::pair = cycles left before branching
    // uptr of std::pair = address to branch to
    std::array<std::pair<sword, uptr>, slots + 1> pending_branches;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(pending_branches[0].first),
            CEREAL_NVP(pending_branches[0].second),
            CEREAL_NVP(pending_branches[1].first),
            CEREAL_NVP(pending_branches[1].second));
    }
};
