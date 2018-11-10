#include "Resources/Ee/Vpu/Vu/VuInstructionDecoder.hpp"

VuInstructionType VU_INSTRUCTION_DECODE_TABLE[Constants::EE::VPU::VU::NUMBER_VU_INSTRUCTIONS] {
    // Upper Instructions
    VuInstructionType::UpperType3,         // ABS
    VuInstructionType::UpperType1,         // ADD
    VuInstructionType::UpperType1,         // ADDi
    VuInstructionType::UpperType1,         // ADDq
    VuInstructionType::UpperType0,         // ADDbc_0
    VuInstructionType::UpperType0,         // ADDbc_1
    VuInstructionType::UpperType0,         // ADDbc_2
    VuInstructionType::UpperType0,         // ADDbc_3
    VuInstructionType::UpperType3_ACC,     // ADDA
    VuInstructionType::UpperType3_ACC,     // ADDAi
    VuInstructionType::UpperType3_ACC,     // ADDAq
    VuInstructionType::UpperType2_ACC,     // ADDAbc_0
    VuInstructionType::UpperType2_ACC,     // ADDAbc_1
    VuInstructionType::UpperType2_ACC,     // ADDAbc_2
    VuInstructionType::UpperType2_ACC,     // ADDAbc_3
    VuInstructionType::UpperType1,         // SUB
    VuInstructionType::UpperType1,         // SUBi
    VuInstructionType::UpperType1,         // SUBq
    VuInstructionType::UpperType0,         // SUBbc_0
    VuInstructionType::UpperType0,         // SUBbc_1
    VuInstructionType::UpperType0,         // SUBbc_2
    VuInstructionType::UpperType0,         // SUBbc_3
    VuInstructionType::UpperType3_ACC,     // SUBA
    VuInstructionType::UpperType3_ACC,     // SUBAi
    VuInstructionType::UpperType3_ACC,     // SUBAq
    VuInstructionType::UpperType2_ACC,     // SUBAbc_0
    VuInstructionType::UpperType2_ACC,     // SUBAbc_1
    VuInstructionType::UpperType2_ACC,     // SUBAbc_2
    VuInstructionType::UpperType2_ACC,     // SUBAbc_3
    VuInstructionType::UpperType1,         // MUL
    VuInstructionType::UpperType1,         // MULi
    VuInstructionType::UpperType1,         // MULq
    VuInstructionType::UpperType0,         // MULbc_0
    VuInstructionType::UpperType0,         // MULbc_1
    VuInstructionType::UpperType0,         // MULbc_2
    VuInstructionType::UpperType0,         // MULbc_3
    VuInstructionType::UpperType3_ACC,     // MULA
    VuInstructionType::UpperType3_ACC,     // MULAi
    VuInstructionType::UpperType3_ACC,     // MULAq
    VuInstructionType::UpperType2_ACC,     // MULAbc_0
    VuInstructionType::UpperType2_ACC,     // MULAbc_1
    VuInstructionType::UpperType2_ACC,     // MULAbc_2
    VuInstructionType::UpperType2_ACC,     // MULAbc_3
    VuInstructionType::UpperType1,         // MADD
    VuInstructionType::UpperType1,         // MADDi
    VuInstructionType::UpperType1,         // MADDq
    VuInstructionType::UpperType0,         // MADDbc_0
    VuInstructionType::UpperType0,         // MADDbc_1
    VuInstructionType::UpperType0,         // MADDbc_2
    VuInstructionType::UpperType0,         // MADDbc_3
    VuInstructionType::UpperType3_ACC,     // MADDA
    VuInstructionType::UpperType3_ACC,     // MADDAi
    VuInstructionType::UpperType3_ACC,     // MADDAq
    VuInstructionType::UpperType2_ACC,     // MADDAbc_0
    VuInstructionType::UpperType2_ACC,     // MADDAbc_1
    VuInstructionType::UpperType2_ACC,     // MADDAbc_2
    VuInstructionType::UpperType2_ACC,     // MADDAbc_3
    VuInstructionType::UpperType1,         // MSUB
    VuInstructionType::UpperType1,         // MSUBi
    VuInstructionType::UpperType1,         // MSUBq
    VuInstructionType::UpperType0,         // MSUBbc_0
    VuInstructionType::UpperType0,         // MSUBbc_1
    VuInstructionType::UpperType0,         // MSUBbc_2
    VuInstructionType::UpperType0,         // MSUBbc_3
    VuInstructionType::UpperType3_ACC,     // MSUBA
    VuInstructionType::UpperType3_ACC,     // MSUBAi
    VuInstructionType::UpperType3_ACC,     // MSUBAq
    VuInstructionType::UpperType2_ACC,     // MSUBAbc_0
    VuInstructionType::UpperType2_ACC,     // MSUBAbc_1
    VuInstructionType::UpperType2_ACC,     // MSUBAbc_2
    VuInstructionType::UpperType2_ACC,     // MSUBAbc_3
    VuInstructionType::UpperType1,         // MAX
    VuInstructionType::UpperType1,         // MAXi
    VuInstructionType::UpperType0,         // MAXbc_0
    VuInstructionType::UpperType0,         // MAXbc_1
    VuInstructionType::UpperType0,         // MAXbc_2
    VuInstructionType::UpperType0,         // MAXbc_3
    VuInstructionType::UpperType1,         // MINI
    VuInstructionType::UpperType1,         // MINIi
    VuInstructionType::UpperType0,         // MINIbc_0
    VuInstructionType::UpperType0,         // MINIbc_1
    VuInstructionType::UpperType0,         // MINIbc_2
    VuInstructionType::UpperType0,         // MINIbc_3
    VuInstructionType::UpperType3_ACC,     // OPMULA
    VuInstructionType::UpperType3_ACC,     // OPMSUB
    VuInstructionType::UpperType3,         // NOP
    VuInstructionType::UpperType3,         // FTOI0
    VuInstructionType::UpperType3,         // FTOI4
    VuInstructionType::UpperType3,         // FTOI12
    VuInstructionType::UpperType3,         // FTOI15
    VuInstructionType::UpperType3,         // ITOF0
    VuInstructionType::UpperType3,         // ITOF4
    VuInstructionType::UpperType3,         // ITOF12
    VuInstructionType::UpperType3,         // ITOF15
    VuInstructionType::UpperType3,         // CLIP
   
    // Lower Instructions              
    VuInstructionType::LowerType4,         // DIV
    VuInstructionType::LowerType4,         // SQRT
    VuInstructionType::LowerType4,         // RSQRT
    VuInstructionType::LowerType1_I,       // IADD
    VuInstructionType::LowerType5,         // IADDI
    VuInstructionType::LowerType8,         // IADDIU
    VuInstructionType::LowerType1_I,       // IAND
    VuInstructionType::LowerType1_I,       // IOR
    VuInstructionType::LowerType1_I,       // ISUB
    VuInstructionType::LowerType8,         // ISUBI
    VuInstructionType::LowerType3,         // MOVE
    VuInstructionType::LowerType3_FT_IS,   // MFIR
    VuInstructionType::LowerType4_IT_FS,   // MTIR
    VuInstructionType::LowerType3,         // MR32
    VuInstructionType::LowerType7_FT_IS,   // LQ
    VuInstructionType::LowerType3_FT_IS,   // LQD
    VuInstructionType::LowerType3_FT_IS,   // LQI
    VuInstructionType::LowerType7,         // SQ
    VuInstructionType::LowerType3_IT_FS,   // SQD
    VuInstructionType::LowerType3_IT_FS,   // SQI
    VuInstructionType::LowerType7_I,       // ILW
    VuInstructionType::LowerType7_I,       // ISW
    VuInstructionType::LowerType3_I,       // ILWR
    VuInstructionType::LowerType3_I,       // ISWR
    VuInstructionType::LowerType4,         // RINIT
    VuInstructionType::LowerType3,         // RGET
    VuInstructionType::LowerType3,         // RNEXT
    VuInstructionType::LowerType4,         // RXOR
    VuInstructionType::LowerType3,         // WAITQ
    VuInstructionType::LowerType8,         // FSAND
    VuInstructionType::LowerType8,         // FSEQ
    VuInstructionType::LowerType8,         // FSOR
    VuInstructionType::LowerType8,         // FSSET
    VuInstructionType::LowerType8,         // FMAND
    VuInstructionType::LowerType8,         // FMEQ
    VuInstructionType::LowerType8,         // FMOR
    VuInstructionType::LowerType9,         // FCAND
    VuInstructionType::LowerType9,         // FCEQ
    VuInstructionType::LowerType9,         // FCOR
    VuInstructionType::LowerType9,         // FCSET
    VuInstructionType::LowerType8,         // FCGET
    VuInstructionType::LowerType7_I,       // IBEQ
    VuInstructionType::LowerType7_I,       // IBGEZ
    VuInstructionType::LowerType7_I,       // IBGTZ
    VuInstructionType::LowerType7_I,       // IBLEZ
    VuInstructionType::LowerType7_I,       // IBLTZ
    VuInstructionType::LowerType7_I,       // IBNE
    VuInstructionType::LowerType7_I,       // B
    VuInstructionType::LowerType7_I,       // BAL
    VuInstructionType::LowerType7_I,       // JR
    VuInstructionType::LowerType7_I,       // JALR
    VuInstructionType::LowerType3,         // MFP
    VuInstructionType::LowerType3,         // WAITP
    VuInstructionType::LowerType3,         // ESADD
    VuInstructionType::LowerType3,         // ERSADD
    VuInstructionType::LowerType3,         // ELENG
    VuInstructionType::LowerType3,         // ERLENG
    VuInstructionType::LowerType3,         // EATANxy
    VuInstructionType::LowerType3,         // EATANxz
    VuInstructionType::LowerType3,         // ESUM
    VuInstructionType::LowerType4,         // ERCPR
    VuInstructionType::LowerType4,         // ESQRT
    VuInstructionType::LowerType4,         // ERSQRT
    VuInstructionType::LowerType4,         // ESIN
    VuInstructionType::LowerType4,         // EATAN
    VuInstructionType::LowerType4,         // EEXP
    VuInstructionType::LowerType3,         // XGKICK
    VuInstructionType::LowerType3_I,       // XTOP
    VuInstructionType::LowerType3_I        // XITOP
};

VuInstructionType VuInstructionDecoder::decode_instruction() const
{
    return VU_INSTRUCTION_DECODE_TABLE[instruction_info.impl_index];
}

std::optional<int> VuInstructionDecoder::try_get_dest() const
{
    VuInstructionType decoded = decode_instruction();

    switch (decoded)
    {
        case VuInstructionType::UpperType0:
        case VuInstructionType::UpperType1:
        case VuInstructionType::LowerType1:
            return instruction.fd();
        
        case VuInstructionType::UpperType2:
        case VuInstructionType::UpperType3:
        case VuInstructionType::LowerType3:
        case VuInstructionType::LowerType7_FT_IS:
            return instruction.ft();

        default:
            return std::nullopt;
    }
}

std::optional<int> VuInstructionDecoder::try_get_dest_field() const
{
    VuInstructionType decoded = decode_instruction();

    switch (decoded)
    {
        case VuInstructionType::UpperType0:
        case VuInstructionType::UpperType1:
        case VuInstructionType::UpperType2:
        case VuInstructionType::UpperType3:
        case VuInstructionType::LowerType1:
        case VuInstructionType::LowerType3:
        case VuInstructionType::LowerType7_FT_IS:
            return instruction.dest();

        case VuInstructionType::LowerType4:
            return 1 << (3 - instruction.ftf());

        default:
            return std::nullopt;
    }
}

std::optional<int> VuInstructionDecoder::try_get_src(int idx) const
{
    VuInstructionType decoded = decode_instruction();

    switch (idx)
    {
        case 0: switch (decoded)
        {
            case VuInstructionType::UpperType0:
            case VuInstructionType::UpperType1:
            case VuInstructionType::UpperType2:
            case VuInstructionType::UpperType3:
            case VuInstructionType::UpperType1_ACC:
            case VuInstructionType::UpperType2_ACC:
            case VuInstructionType::UpperType3_ACC:
            case VuInstructionType::LowerType1:
            case VuInstructionType::LowerType3:
            case VuInstructionType::LowerType3_IT_FS:
            case VuInstructionType::LowerType4:
            case VuInstructionType::LowerType4_IT_FS:
            case VuInstructionType::LowerType7:
            case VuInstructionType::LowerType8:
                return instruction.fs();

            default:
                return std::nullopt;
        };

        case 1: switch (decoded)
        {
            case VuInstructionType::UpperType0:
            case VuInstructionType::UpperType1:
            case VuInstructionType::UpperType1_ACC:
            case VuInstructionType::UpperType2:
            case VuInstructionType::UpperType2_ACC:
            case VuInstructionType::UpperType3:
            case VuInstructionType::UpperType3_ACC:
            case VuInstructionType::LowerType1:
            case VuInstructionType::LowerType4:
                return instruction.ft();

            default:
                return std::nullopt;
        };

        default:
            return std::nullopt;
    }
}

std::optional<int> VuInstructionDecoder::try_get_src_field(int idx) const
{
    VuInstructionType decoded = decode_instruction();

    switch (idx)
    {
        case 0: switch (decoded)
        {
            case VuInstructionType::UpperType0:
            case VuInstructionType::UpperType1:
            case VuInstructionType::UpperType2:
            case VuInstructionType::UpperType3:
            case VuInstructionType::UpperType1_ACC:
            case VuInstructionType::UpperType2_ACC:
            case VuInstructionType::UpperType3_ACC:
            case VuInstructionType::LowerType1:
            case VuInstructionType::LowerType3:
            case VuInstructionType::LowerType3_IT_FS:
            case VuInstructionType::LowerType7:
            case VuInstructionType::LowerType8:
                return instruction.dest();

            case VuInstructionType::LowerType4:
            case VuInstructionType::LowerType4_IT_FS:
                return 1 << (3 - instruction.fsf());

            default:
                return std::nullopt;
        }

        case 1: switch (decoded)
        {
            case VuInstructionType::UpperType0:
            case VuInstructionType::UpperType2:
            case VuInstructionType::UpperType2_ACC:
                return 1 << (3 - instruction.bc());

            case VuInstructionType::UpperType1:
            case VuInstructionType::UpperType1_ACC:
            case VuInstructionType::UpperType3:
            case VuInstructionType::UpperType3_ACC:
            case VuInstructionType::LowerType1:
                return instruction.dest();

            case VuInstructionType::LowerType4:
                return 1 << (3 - instruction.ftf());

            default:
                return std::nullopt;
        }

        default:
            return std::nullopt;
    }
}


bool VuInstructionDecoder::is_integer_instruction() const
{
    VuInstructionType decoded = decode_instruction();

    switch (decoded)
    {
        case VuInstructionType::LowerType1:
        case VuInstructionType::LowerType3_I:
        case VuInstructionType::LowerType3_IT_FS:
        case VuInstructionType::LowerType4_IT_FS:
        case VuInstructionType::LowerType5:
        case VuInstructionType::LowerType7:
        case VuInstructionType::LowerType7_I:
        case VuInstructionType::LowerType8:
            return true;

        default:
            return false;
    }
}
