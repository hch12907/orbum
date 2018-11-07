#pragma once

#include <optional>
#include <variant>

#include "Common/Types/Bitfield.hpp"
#include "Resources/Ee/Vpu/Vu/VuInstruction.hpp"

/// Used to store the type of the instruction. The type determines the fields in
/// which the instruction is going to have. This is required for VU data hazards
/// checking.
/// Note: Some instructions use the register fields differently, although the
///       formats are considered the same.
/// Note: OPCODE and OPCODE+BC are both 6 bits, OPCODE+0b1111+[*] is 11 bits.
enum class VuInstructionType
{
    UpperType0,       // DEST, FT, FS, FD, OPCODE+BC
    UpperType1,       // DEST, FT, FS, FD, OPCODE
    UpperType2,       // DEST, FT, FS, OPCODE+0b1111+BC
    UpperType3,       // DEST, FT, FS, OPCODE+0b1111+OPCODE
    UpperType1_ACC,   // DEST, FT, FS, FD, OPCODE; uses ACC
    UpperType2_ACC,   // DEST, FT, FS, OPCODE+0b1111+BC; uses ACC
    UpperType3_ACC,   // DEST, FT, FS, OPCODE+0b1111+OPCODE; uses ACC

    LowerType1,       // DEST, FT, FS, FD, OPCODE
    LowerType1_I,     // DEST, IT, IS, ID, OPCODE
    LowerType2,       // (unspecified)
    LowerType3,       // DEST, FT, FS, OPCODE+0b1111+OPCODE
    LowerType3_I,     // DEST, IT, IS, OPCODE+0b1111+OPCODE
    LowerType3_IT_FS, // DEST, IT, FS, OPCODE+0b1111+OPCODE
    LowerType3_FT_IS, // DEST, FT, IS, OPCODE+0b1111+OPCODE
    LowerType4,       // FTF+FSF, FT, FS, OPCODE+0b1111+OPCODE
    LowerType4_IT_FS, // FTF+FSF, IT, FS, OPCODE+0b1111+OPCODE
    LowerType5,       // DEST, IT, IS, IMM5, OPCODE
    LowerType6,       // (unspecified)
    LowerType7,       // DEST, IT, FS, IMM11
    LowerType7_I,     // DEST, IT, IS, IMM11
    LowerType7_FT_IS, // DEST, FT, IS, IMM11
    LowerType8,       // LOWER OP, IMM15, IT, IS, IMM15
    LowerType9,       // LOWER OP, 0b0, IMM24
};

/// The name says all. Contains some helper functions to ease instruction
/// decoding.
class VuInstructionDecoder
{
public:
    VuInstructionDecoder(const VuInstruction inst, const MipsInstructionInfo info) :
        instruction(inst),
        instruction_info(info)
    {
    }

    /// Decodes the instruction, returning the type of the instruction.
    /// See VuInstructionType for more details.
    VuInstructionType decode_instruction() const;

    /// Obtains the location of destination register, if it is specified by 
    /// the instruction.
    /// Note: This function returns nullopt for non-VF/non-VI destinations.
    std::optional<int> try_get_dest() const;

    /// Obtains the field specified by the instruction.
    std::optional<int> try_get_dest_field() const;

    /// Obtains the location of source register, if specified by the instruction.
    /// Some instructions specify 2 or more registers, therefore idx is required
    /// to specify which source register to obtain.
    /// Note: This function returns nullopt for special sources.
    std::optional<int> try_get_src(int idx) const;

    /// Obtains the field specified by the instruction.
    std::optional<int> try_get_src_field(int idx) const;

    /// Returns true if the instruction uses VI registers as its destination.
    bool is_integer_instruction() const;

    VuInstruction instruction;
    MipsInstructionInfo instruction_info;
};
