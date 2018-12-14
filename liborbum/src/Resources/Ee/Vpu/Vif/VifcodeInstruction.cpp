#include "Resources/Ee/Vpu/Vif/VifcodeInstruction.hpp"

MipsInstructionInfo VIFCODE_INSTRUCTION_TABLE[34] =
    {
        {"NOP", 1, 1},
        {"STCYCL", 2, 1},
        {"OFFSET", 3, 1},
        {"BASE", 4, 1},
        {"ITOP", 5, 1},
        {"STMOD", 6, 1},
        {"MSKPATH3", 7, 1},
        {"MARK", 8, 1},
        {"FLUSHE", 9, 1},
        {"FLUSH", 10, 1},
        {"FLUSHA", 11, 1},
        {"MSCAL", 12, 1},
        {"MSCALF", 13, 1},
        {"MSCNT", 14, 1},
        {"STMASK", 15, 2},
        {"STROW", 16, 5},
        {"STCOL", 17, 5},
        {"MPG", 18, SpecialVifcodePacketUsage::Num},
        {"DIRECT", 19, SpecialVifcodePacketUsage::Immediate},
        {"DIRECTHL", 20, SpecialVifcodePacketUsage::Immediate},
        {"UNPACK_S_32", 21, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_S_16", 22, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_S_8", 23, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V2_32", 24, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V2_16", 25, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V2_8", 26, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V3_32", 27, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V3_16", 28, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V3_8", 29, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V4_32", 30, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V4_16", 31, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V4_8", 32, SpecialVifcodePacketUsage::Unpack},
        {"UNPACK_V4_5", 33, SpecialVifcodePacketUsage::Unpack},
        {"UNKNOWN", 0, 1}};

VifcodeInstruction::VifcodeInstruction(const uword value) :
    MipsInstruction(value),
    info(nullptr)
{
}

MipsInstructionInfo* VifcodeInstruction::lookup() const
{
    switch (cmdhi())
    {
    case 0:
    {
        switch (cmdlo())
        {
        case 0:
            return &VIFCODE_INSTRUCTION_TABLE[0];
        case 1:
            return &VIFCODE_INSTRUCTION_TABLE[1];
        case 2:
            return &VIFCODE_INSTRUCTION_TABLE[2];
        case 3:
            return &VIFCODE_INSTRUCTION_TABLE[3];
        case 4:
            return &VIFCODE_INSTRUCTION_TABLE[4];
        case 5:
            return &VIFCODE_INSTRUCTION_TABLE[5];
        case 6:
            return &VIFCODE_INSTRUCTION_TABLE[6];
        case 7:
            return &VIFCODE_INSTRUCTION_TABLE[7];
        case 16:
            return &VIFCODE_INSTRUCTION_TABLE[8];
        case 17:
            return &VIFCODE_INSTRUCTION_TABLE[9];
        case 19:
            return &VIFCODE_INSTRUCTION_TABLE[10];
        case 20:
            return &VIFCODE_INSTRUCTION_TABLE[11];
        case 21:
            return &VIFCODE_INSTRUCTION_TABLE[12];
        case 23:
            return &VIFCODE_INSTRUCTION_TABLE[13];
        }
    }
    case 1:
    {
        switch (cmdlo())
        {
        case 0:
            return &VIFCODE_INSTRUCTION_TABLE[14];
        case 16:
            return &VIFCODE_INSTRUCTION_TABLE[15];
        case 17:
            return &VIFCODE_INSTRUCTION_TABLE[16];
        }
    }
    case 2:
    {
        switch (cmdlo())
        {
        case 10:
            return &VIFCODE_INSTRUCTION_TABLE[17];
        case 16:
            return &VIFCODE_INSTRUCTION_TABLE[18];
        case 17:
            return &VIFCODE_INSTRUCTION_TABLE[19];
        }
    }
    case 3:
    {
        switch (cmdlo() & 0xF) // ignore M bit
        {
        case 0:
            return &VIFCODE_INSTRUCTION_TABLE[20];
        case 1:
            return &VIFCODE_INSTRUCTION_TABLE[21];
        case 2:
            return &VIFCODE_INSTRUCTION_TABLE[22];
        case 4:
            return &VIFCODE_INSTRUCTION_TABLE[23];
        case 5:
            return &VIFCODE_INSTRUCTION_TABLE[24];
        case 6:
            return &VIFCODE_INSTRUCTION_TABLE[25];
        case 8:
            return &VIFCODE_INSTRUCTION_TABLE[26];
        case 9:
            return &VIFCODE_INSTRUCTION_TABLE[27];
        case 10:
            return &VIFCODE_INSTRUCTION_TABLE[28];
        case 12:
            return &VIFCODE_INSTRUCTION_TABLE[29];
        case 13:
            return &VIFCODE_INSTRUCTION_TABLE[30];
        case 14:
            return &VIFCODE_INSTRUCTION_TABLE[31];
        case 15:
            return &VIFCODE_INSTRUCTION_TABLE[32];
        }
    }
    default:
        return &VIFCODE_INSTRUCTION_TABLE[33];
    }
}
