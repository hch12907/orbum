#pragma once

#include "Common/Types/Mips/MipsInstruction.hpp"
#include "Common/Types/Mips/MipsInstructionInfo.hpp"
#include "Common/Types/Primitive.hpp"

/// Some VIFcode use an arbitrary amount of packets (such as MPG) while others
/// use a fixed amount of packets. Thus, for those special VIFcodes we use magic
/// numbers to store their packet length. (100 and above ought to be enough... right?)
struct SpecialVifcodePacketUsage
{
    enum {
        Num = 100,
        Immediate = 200,
        Unpack = 300, 
    };
};

/// A VIFcode type, as explained on page 87 of the EE Users Manual.
/// Although a VIF transfer packet is 128-bit long, the VIFcode part is 32-bit.
struct VifcodeInstruction : public MipsInstruction
{
    static constexpr Bitfield IMM = Bitfield(0, 16);
    static constexpr Bitfield NUM = Bitfield(16, 8);
    static constexpr Bitfield CMD = Bitfield(24, 8);
    static constexpr Bitfield CMDLO = Bitfield(24, 5);
    static constexpr Bitfield CMDHI = Bitfield(29, 2);
    static constexpr Bitfield I = Bitfield(31, 1);

    // Used by UNPACK
    static constexpr Bitfield VL = Bitfield(24, 2);
    static constexpr Bitfield VN = Bitfield(26, 2);
    static constexpr Bitfield M = Bitfield(28, 1);

    VifcodeInstruction(const uword value);

    /// Field extraction functions.
    /// See documentation above for the actual bits.
    uhword imm() const
    {
        return static_cast<uhword>(IMM.extract_from(value));
    }

    ubyte num() const
    {
        return static_cast<ubyte>(NUM.extract_from(value));
    }

    ubyte cmd() const
    {
        return static_cast<ubyte>(CMD.extract_from(value));
    }

    ubyte cmdlo() const
    {
        return static_cast<ubyte>(CMDLO.extract_from(value));
    }

    ubyte cmdhi() const
    {
        return static_cast<ubyte>(CMDHI.extract_from(value));
    }

    ubyte i() const
    {
        return static_cast<ubyte>(I.extract_from(value));
    }

    ubyte m() const
    {
        return static_cast<ubyte>(M.extract_from(value));
    }

    ubyte vl() const
    {
        return static_cast<ubyte>(VL.extract_from(value));
    }

    ubyte vn() const
    {
        return static_cast<ubyte>(VN.extract_from(value));
    }

    /// Performs a lookup if required and returns the instruction details.
    const MipsInstructionInfo* get_info() const
    {
        if (!info)
            info = lookup();
        return info;
    }

private:
    /// Instruction information (from performing lookup).
    mutable MipsInstructionInfo* info;

    /// Determines what instruction this is by performing a lookup.
    MipsInstructionInfo* lookup() const;
};
