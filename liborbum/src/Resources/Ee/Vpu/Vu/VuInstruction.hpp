#pragma once

#include "Common/Constants.hpp"
#include "Common/Types/Mips/MipsInstruction.hpp"
#include "Common/Types/Mips/MipsInstructionInfo.hpp"
#include "Resources/Ee/Vpu/Vu/VuVectorField.hpp"

/// A VU instruction type, which is used to extract information out of the parsed 32-bit value.
/// See VU Users Manual page 58 onwards (micro) & page 226 onwards (macro).
/// The micro and macro instruction types are compatible with eachother as they both use 32-bit values.
/// However, the field names are from the micro series of instructions.
/// This is very similar to a MIPS instruction type.
/// In the system code, you must split the full instruction (64-bit) value into
/// 2 x 32-bit values, from which you can use this.
struct VuInstruction : public MipsInstruction
{
    static constexpr Bitfield MSB7 = Bitfield(25, 7);
    static constexpr Bitfield DEST = Bitfield(21, 4);
    static constexpr Bitfield FT = Bitfield(16, 5);
    static constexpr Bitfield FS = Bitfield(11, 5);
    static constexpr Bitfield FD = Bitfield(6, 5);
    static constexpr Bitfield OPCODE = Bitfield(0, 6);
    static constexpr Bitfield BC = Bitfield(0, 2);
    static constexpr Bitfield FTF = Bitfield(23, 2);
    static constexpr Bitfield FSF = Bitfield(21, 2);
    static constexpr Bitfield IMM11 = Bitfield(0, 11);
    static constexpr Bitfield IMM24 = Bitfield(0, 24);
    static constexpr Bitfield X = Bitfield(24, 1);
    static constexpr Bitfield Y = Bitfield(23, 1);
    static constexpr Bitfield Z = Bitfield(22, 1);
    static constexpr Bitfield W = Bitfield(21, 1);

    VuInstruction(const uword value) :
        MipsInstruction(value),
        lower_info(nullptr),
        upper_info(nullptr)
    {
    }

    /// Field extraction functions.
    /// See documentation above for the actual bits.
    ubyte msb7() const
    {
        return static_cast<ubyte>(MSB7.extract_from(value));
    }

    ubyte dest() const
    {
        return static_cast<ubyte>(DEST.extract_from(value));
    }

    ubyte ft() const
    {
        return static_cast<ubyte>(FT.extract_from(value));
    }

    inline ubyte it() const
    {
        // FT and IT differ in name only
        return ft();
    }

    ubyte fs() const
    {
        return static_cast<ubyte>(FS.extract_from(value));
    }

    inline ubyte is() const
    {
        // FS and IS differ in name only
        return fs();
    }

    ubyte fd() const
    {
        return static_cast<ubyte>(FD.extract_from(value));
    }

    inline ubyte id() const
    {
        // FD and ID differ in name only
        return fs();
    }

    ubyte opcode() const
    {
        return static_cast<ubyte>(OPCODE.extract_from(value));
    }

    ubyte bc() const
    {
        return static_cast<ubyte>(BC.extract_from(value));
    }

    ubyte ftf() const
    {
        return static_cast<ubyte>(FTF.extract_from(value));
    }

    ubyte fsf() const
    {
        return static_cast<ubyte>(FSF.extract_from(value));
    }

    ubyte imm5() const
    {
        // FD is IMM5 in some instructions
        return static_cast<ubyte>(FD.extract_from(value));
    }

    uhword imm11() const
    {
        return static_cast<uhword>(IMM11.extract_from(value));
    }

    uhword imm12() const
    {
        return static_cast<uhword>(IMM11.extract_from(value) | (DEST.extract_from(value) & 1) << 11);
    }

    uhword imm15() const
    {
        return static_cast<uhword>(IMM11.extract_from(value) | (DEST.extract_from(value) << 11));
    }

    uword imm24() const
    {
        return static_cast<uword>(IMM24.extract_from(value));
    }

    /// Test functions for the subfields of the dest field (x, y, z, w).
    /// Returns if the subfield bit is set.
    /// The field index/bits/subfield map is as follows:
    /// Index Parm. 3  ->  0.
    /// Bits        21 ->  24.
    /// Subfield    W  ->  X.
    /// See VU Users Manual page 61.
    bool test_dest_field(const VuVectorField::Field field) const
    {
        if (field == VuVectorField::Field::X)
            return test_dest_x();
        else if (field == VuVectorField::Field::Y)
            return test_dest_y();
        else if (field == VuVectorField::Field::Z)
            return test_dest_z();
        else if (field == VuVectorField::Field::W)
            return test_dest_w();
        else
            throw std::runtime_error("Invalid field to test for in VU Instruction - please debug!");
    }

    bool test_dest_x() const
    {
        return X.extract_from(value) > 0;
    }

    bool test_dest_y() const
    {
        return Y.extract_from(value) > 0;
    }

    bool test_dest_z() const
    {
        return Z.extract_from(value) > 0;
    }

    bool test_dest_w() const
    {
        return W.extract_from(value) > 0;
    }

    MipsInstructionInfo& get_lower_info() const;
    MipsInstructionInfo& get_upper_info() const;

private:
    MipsInstructionInfo* lower_lookup() const;
    MipsInstructionInfo* upper_lookup() const;

    mutable MipsInstructionInfo* lower_info;
    mutable MipsInstructionInfo* upper_info;
};
