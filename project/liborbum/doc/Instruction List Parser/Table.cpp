MipsInstructionInfo INSTRUCTION_TABLE[386] =
{
    { "SLL", 48, CPI_R5900_DEFAULT },
    { "SRL", 49, CPI_R5900_DEFAULT },
    { "SRA", 50, CPI_R5900_DEFAULT },
    { "SLLV", 51, CPI_R5900_DEFAULT },
    { "SRLV", 52, CPI_R5900_DEFAULT },
    { "SRAV", 53, CPI_R5900_DEFAULT },
    { "JR", 54, CPI_R5900_BRANCH },
    { "JALR", 55, CPI_R5900_BRANCH },
    { "MOVZ", 56, CPI_R5900_DEFAULT },
    { "MOVN", 57, CPI_R5900_DEFAULT },
    { "SYSCALL", 58, CPI_R5900_DEFAULT },
    { "BREAK", 59, CPI_R5900_DEFAULT },
    { "SYNC", 60, CPI_R5900_DEFAULT },
    { "MFHI", 61, CPI_R5900_DEFAULT },
    { "MTHI", 62, CPI_R5900_DEFAULT },
    { "MFLO", 63, CPI_R5900_DEFAULT },
    { "MTLO", 64, CPI_R5900_DEFAULT },
    { "DSLLV", 65, CPI_R5900_DEFAULT },
    { "DSRLV", 66, CPI_R5900_DEFAULT },
    { "DSRAV", 67, CPI_R5900_DEFAULT },
    { "MULT", 68, CPI_R5900_MULTIPLY },
    { "MULTU", 69, CPI_R5900_MULTIPLY },
    { "DIV", 70, CPI_R5900_DIVIDE },
    { "DIVU", 71, CPI_R5900_DIVIDE },
    { "ADD", 72, CPI_R5900_DEFAULT },
    { "ADDU", 73, CPI_R5900_DEFAULT },
    { "SUB", 74, CPI_R5900_DEFAULT },
    { "SUBU", 75, CPI_R5900_DEFAULT },
    { "AND", 76, CPI_R5900_DEFAULT },
    { "OR", 77, CPI_R5900_DEFAULT },
    { "XOR", 78, CPI_R5900_DEFAULT },
    { "NOR", 79, CPI_R5900_DEFAULT },
    { "MFSA", 80, CPI_R5900_DEFAULT },
    { "MTSA", 81, CPI_R5900_DEFAULT },
    { "SLT", 82, CPI_R5900_DEFAULT },
    { "SLTU", 83, CPI_R5900_DEFAULT },
    { "DADD", 84, CPI_R5900_DEFAULT },
    { "DADDU", 85, CPI_R5900_DEFAULT },
    { "DSUB", 86, CPI_R5900_DEFAULT },
    { "DSUBU", 87, CPI_R5900_DEFAULT },
    { "TGE", 88, CPI_R5900_BRANCH },
    { "TGEU", 89, CPI_R5900_BRANCH },
    { "TLT", 90, CPI_R5900_BRANCH },
    { "TLTU", 91, CPI_R5900_BRANCH },
    { "TEQ", 92, CPI_R5900_BRANCH },
    { "TNE", 93, CPI_R5900_BRANCH },
    { "DSLL", 94, CPI_R5900_DEFAULT },
    { "DSRL", 95, CPI_R5900_DEFAULT },
    { "DSRA", 96, CPI_R5900_DEFAULT },
    { "DSLL32", 97, CPI_R5900_DEFAULT },
    { "DSRL32", 98, CPI_R5900_DEFAULT },
    { "DSRA32", 99, CPI_R5900_DEFAULT },
    { "BLTZ", 100, CPI_R5900_BRANCH },
    { "BGEZ", 101, CPI_R5900_BRANCH },
    { "BLTZL", 102, CPI_R5900_BRANCH },
    { "BGEZL", 103, CPI_R5900_BRANCH },
    { "TGEI", 104, CPI_R5900_BRANCH },
    { "TGEIU", 105, CPI_R5900_BRANCH },
    { "TLTI", 106, CPI_R5900_BRANCH },
    { "TLTIU", 107, CPI_R5900_BRANCH },
    { "TEQI", 108, CPI_R5900_BRANCH },
    { "TNEI", 109, CPI_R5900_BRANCH },
    { "BLTZAL", 110, CPI_R5900_BRANCH },
    { "TGEZAL", 111, CPI_R5900_BRANCH },
    { "BLTZALL", 112, CPI_R5900_BRANCH },
    { "BGEZALL", 113, CPI_R5900_BRANCH },
    { "MTSAB", 114, CPI_R5900_BRANCH },
    { "MTSAH", 115, CPI_R5900_BRANCH },
    { "PADDW", 137, CPI_MMI_DEFAULT },
    { "PSUBW", 138, CPI_MMI_DEFAULT },
    { "PCGTW", 139, CPI_MMI_DEFAULT },
    { "PMAXW", 140, CPI_MMI_DEFAULT },
    { "PADDH", 141, CPI_MMI_DEFAULT },
    { "PSUBH", 142, CPI_MMI_DEFAULT },
    { "PCGTH", 143, CPI_MMI_DEFAULT },
    { "PMAXH", 144, CPI_MMI_DEFAULT },
    { "PADDB", 145, CPI_MMI_DEFAULT },
    { "PSUBB", 146, CPI_MMI_DEFAULT },
    { "PCGTB", 147, CPI_MMI_DEFAULT },
    { "PADDSW", 148, CPI_MMI_DEFAULT },
    { "PSUBSW", 149, CPI_MMI_DEFAULT },
    { "PEXTLW", 150, CPI_MMI_DEFAULT },
    { "PPACW", 151, CPI_MMI_DEFAULT },
    { "PADDSH", 152, CPI_MMI_DEFAULT },
    { "PSUBSH", 153, CPI_MMI_DEFAULT },
    { "PEXTLH", 154, CPI_MMI_DEFAULT },
    { "PPACH", 155, CPI_MMI_DEFAULT },
    { "PADDSB", 156, CPI_MMI_DEFAULT },
    { "PSUBSB", 157, CPI_MMI_DEFAULT },
    { "PEXTLB", 158, CPI_MMI_DEFAULT },
    { "PPACB", 159, CPI_MMI_DEFAULT },
    { "PEXT5", 160, CPI_MMI_DEFAULT },
    { "PPAC5", 161, CPI_MMI_DEFAULT },
    { "PABSW", 162, CPI_MMI_DEFAULT },
    { "PCEQW", 163, CPI_MMI_DEFAULT },
    { "PMINW", 164, CPI_MMI_DEFAULT },
    { "PADSBH", 165, CPI_MMI_DEFAULT },
    { "PABSH", 166, CPI_MMI_DEFAULT },
    { "PCEQH", 167, CPI_MMI_DEFAULT },
    { "PMINH", 168, CPI_MMI_DEFAULT },
    { "PCEQB", 169, CPI_MMI_DEFAULT },
    { "PADDUW", 170, CPI_MMI_DEFAULT },
    { "PSUBUW", 171, CPI_MMI_DEFAULT },
    { "PEXTUW", 172, CPI_MMI_DEFAULT },
    { "PADDUH", 173, CPI_MMI_DEFAULT },
    { "PSUBUH", 174, CPI_MMI_DEFAULT },
    { "PEXTUH", 175, CPI_MMI_DEFAULT },
    { "PADDUB", 176, CPI_MMI_DEFAULT },
    { "PSUBUB", 177, CPI_MMI_DEFAULT },
    { "PEXTUB", 178, CPI_MMI_DEFAULT },
    { "QFSRV", 179, CPI_MMI_DEFAULT },
    { "PMADDW", 180, CPI_MMI_DEFAULT },
    { "PSLLVW", 181, CPI_MMI_DEFAULT },
    { "PSRLVW", 182, CPI_MMI_DEFAULT },
    { "PMSUBW", 183, CPI_MMI_DEFAULT },
    { "PMFHI", 184, CPI_MMI_DEFAULT },
    { "PMFLO", 185, CPI_MMI_DEFAULT },
    { "PINTH", 186, CPI_MMI_DEFAULT },
    { "PMULTW", 187, CPI_MMI_DEFAULT },
    { "PDIVW", 188, CPI_MMI_DEFAULT },
    { "PCPYLD", 189, CPI_MMI_DEFAULT },
    { "PMADDH", 190, CPI_MMI_DEFAULT },
    { "PHMADH", 191, CPI_MMI_DEFAULT },
    { "PAND", 192, CPI_MMI_DEFAULT },
    { "PXOR", 193, CPI_MMI_DEFAULT },
    { "PMSUBH", 194, CPI_MMI_DEFAULT },
    { "PHMSBH", 195, CPI_MMI_DEFAULT },
    { "PEXEH", 196, CPI_MMI_DEFAULT },
    { "PREVH", 197, CPI_MMI_DEFAULT },
    { "PMULTH", 198, CPI_MMI_DEFAULT },
    { "PDIVBW", 199, CPI_MMI_DEFAULT },
    { "PEXEW", 200, CPI_MMI_DEFAULT },
    { "PROT3W", 201, CPI_MMI_DEFAULT },
    { "PMADDUW", 202, CPI_MMI_DEFAULT },
    { "PSRAVW", 203, CPI_MMI_DEFAULT },
    { "PMTHI", 204, CPI_MMI_DEFAULT },
    { "PMTLO", 205, CPI_MMI_DEFAULT },
    { "PINTEH", 206, CPI_MMI_DEFAULT },
    { "PMULTUW", 207, CPI_MMI_DEFAULT },
    { "PDIVUW", 208, CPI_MMI_DEFAULT },
    { "PCPYUD", 209, CPI_MMI_DEFAULT },
    { "POR", 210, CPI_MMI_DEFAULT },
    { "PNOR", 211, CPI_MMI_DEFAULT },
    { "PEXCH", 212, CPI_MMI_DEFAULT },
    { "PCPYH", 213, CPI_MMI_DEFAULT },
    { "PEXCW", 214, CPI_MMI_DEFAULT },
    { "MADD", 116, CPI_MMI_DEFAULT },
    { "MADDU", 117, CPI_MMI_DEFAULT },
    { "PLZCW", 118, CPI_MMI_DEFAULT },
    { "MFHI1", 119, CPI_R5900_DEFAULT },
    { "MTHI1", 120, CPI_R5900_DEFAULT },
    { "MFLO1", 121, CPI_R5900_DEFAULT },
    { "MTLO1", 122, CPI_R5900_DEFAULT },
    { "MULT1", 123, CPI_R5900_MULTIPLY },
    { "MULTU1", 124, CPI_R5900_MULTIPLY },
    { "DIV1", 125, CPI_R5900_DIVIDE },
    { "DIVU1", 126, CPI_R5900_DIVIDE },
    { "MADD1", 127, CPI_MMI_DEFAULT },
    { "MADDU1", 128, CPI_MMI_DEFAULT },
    { "PMFHL", 129, CPI_MMI_DEFAULT },
    { "PMTHL", 130, CPI_MMI_DEFAULT },
    { "PSLLH", 131, CPI_MMI_DEFAULT },
    { "PSRLH", 132, CPI_MMI_DEFAULT },
    { "PSRAH", 133, CPI_MMI_DEFAULT },
    { "PSLLW", 134, CPI_MMI_DEFAULT },
    { "PSRLW", 135, CPI_MMI_DEFAULT },
    { "PSRAW", 136, CPI_MMI_DEFAULT },
    { "BC0F", 217, CPI_COP_BRANCH_DELAY },
    { "BC0T", 218, CPI_COP_BRANCH_DELAY },
    { "BC0FL", 219, CPI_COP_BRANCH_DELAY_LIKELY },
    { "BC0TL", 220, CPI_COP_BRANCH_DELAY_LIKELY },
    { "TLBR", 221, CPI_COP_DEFAULT },
    { "TLBWI", 222, CPI_COP_DEFAULT },
    { "TLBWR", 223, CPI_COP_DEFAULT },
    { "TLBP", 224, CPI_COP_DEFAULT },
    { "ERET", 225, CPI_COP_DEFAULT },
    { "EI", 226, CPI_COP_DEFAULT },
    { "DI", 227, CPI_COP_DEFAULT },
    { "MFC0", 215, CPI_COP_DEFAULT },
    { "MTC0", 216, CPI_COP_DEFAULT },
    { "BC1F", 232, CPI_COP_DEFAULT },
    { "BC1T", 233, CPI_COP_BRANCH_DELAY },
    { "BC1FL", 234, CPI_COP_BRANCH_DELAY },
    { "BC1TL", 235, CPI_COP_BRANCH_DELAY_LIKELY },
    { "ADD_S", 236, CPI_COP_DEFAULT },
    { "SUB_S", 237, CPI_COP_DEFAULT },
    { "MUL_S", 238, CPI_COP_DEFAULT },
    { "DIV_S", 239, CPI_COP_DEFAULT },
    { "SQRT_S", 240, CPI_COP_DEFAULT },
    { "ABS_S", 241, CPI_COP_DEFAULT },
    { "MOV_S", 242, CPI_COP_DEFAULT },
    { "NEG_S", 243, CPI_COP_DEFAULT },
    { "RSQRT_S", 244, CPI_COP_DEFAULT },
    { "ADDA_S", 245, CPI_COP_DEFAULT },
    { "SUBA_S", 246, CPI_COP_DEFAULT },
    { "MULA_S", 247, CPI_COP_DEFAULT },
    { "MADD_S", 248, CPI_COP_DEFAULT },
    { "MSUB_S", 249, CPI_COP_DEFAULT },
    { "MADDA_S", 250, CPI_COP_DEFAULT },
    { "MSUBA_S", 251, CPI_COP_DEFAULT },
    { "CVTW_S", 252, CPI_COP_DEFAULT },
    { "MAX_S", 253, CPI_COP_DEFAULT },
    { "MIN_S", 254, CPI_COP_DEFAULT },
    { "C.F_S", 255, CPI_COP_DEFAULT },
    { "C.EQ_S", 256, CPI_COP_DEFAULT },
    { "C.LT_S", 257, CPI_COP_DEFAULT },
    { "C.LE_S", 258, CPI_COP_DEFAULT },
    { "CVTS_S", 259, CPI_COP_DEFAULT },
    { "MFC1", 228, CPI_COP_DEFAULT },
    { "CFC1", 229, CPI_COP_DEFAULT },
    { "MTC1", 230, CPI_COP_DEFAULT },
    { "CTC1", 231, CPI_COP_DEFAULT },
    { "BC2F", 264, CPI_COP_DEFAULT },
    { "BC2T", 265, CPI_COP_DEFAULT },
    { "BC2FL", 266, CPI_COP_DEFAULT },
    { "BC2TL", 267, CPI_COP_DEFAULT },
    { "QMFC2", 260, CPI_COP_DEFAULT },
    { "CFC2", 261, CPI_COP_DEFAULT },
    { "QMTC2", 262, CPI_COP_DEFAULT },
    { "CTC2", 263, CPI_COP_DEFAULT },
    { "VADDAbc.0", 323, CPI_COP_DEFAULT },
    { "VSUBAbc.0", 324, CPI_COP_DEFAULT },
    { "VMADDAbc.0", 325, CPI_COP_DEFAULT },
    { "VMSUBAbc.0", 326, CPI_COP_DEFAULT },
    { "VITOF0", 327, CPI_COP_DEFAULT },
    { "VFTOI0", 328, CPI_COP_DEFAULT },
    { "VMULAbc.0", 329, CPI_COP_DEFAULT },
    { "VMULAq", 330, CPI_COP_DEFAULT },
    { "VADDAq", 331, CPI_COP_DEFAULT },
    { "VSUBAq", 332, CPI_COP_DEFAULT },
    { "VADDA", 333, CPI_COP_DEFAULT },
    { "VSUBA", 334, CPI_COP_DEFAULT },
    { "VMOVE", 335, CPI_COP_DEFAULT },
    { "VLQI", 336, CPI_COP_DEFAULT },
    { "VDIV", 337, CPI_COP_DEFAULT },
    { "VMTIR", 338, CPI_COP_DEFAULT },
    { "VRNEXT", 339, CPI_COP_DEFAULT },
    { "VADDAbc.1", 340, CPI_COP_DEFAULT },
    { "VSUBAbc.1", 341, CPI_COP_DEFAULT },
    { "VMADDAbc.1", 342, CPI_COP_DEFAULT },
    { "VMSUBAbc.1", 343, CPI_COP_DEFAULT },
    { "VITOF4", 344, CPI_COP_DEFAULT },
    { "VFTIO4", 345, CPI_COP_DEFAULT },
    { "VMULAbc.1", 346, CPI_COP_DEFAULT },
    { "VABS", 347, CPI_COP_DEFAULT },
    { "VMADDAq", 348, CPI_COP_DEFAULT },
    { "VMSUBAq", 349, CPI_COP_DEFAULT },
    { "VMADDA", 350, CPI_COP_DEFAULT },
    { "VMSUBA", 351, CPI_COP_DEFAULT },
    { "VMR32", 352, CPI_COP_DEFAULT },
    { "VSQI", 353, CPI_COP_DEFAULT },
    { "VSQRT", 354, CPI_COP_DEFAULT },
    { "VMFIR", 355, CPI_COP_DEFAULT },
    { "VRGET", 356, CPI_COP_DEFAULT },
    { "VADDAbc.2", 357, CPI_COP_DEFAULT },
    { "VSUBAbc.2", 358, CPI_COP_DEFAULT },
    { "VMADDAbc.2", 359, CPI_COP_DEFAULT },
    { "VMSUBAbc.2", 360, CPI_COP_DEFAULT },
    { "VITOF12", 361, CPI_COP_DEFAULT },
    { "VFTIO12", 362, CPI_COP_DEFAULT },
    { "VMULAbc.2", 363, CPI_COP_DEFAULT },
    { "VMULAi", 364, CPI_COP_DEFAULT },
    { "VADDAi", 365, CPI_COP_DEFAULT },
    { "VSUBAi", 366, CPI_COP_DEFAULT },
    { "VMULA", 367, CPI_COP_DEFAULT },
    { "VOPMULA", 368, CPI_COP_DEFAULT },
    { "VLQD", 369, CPI_COP_DEFAULT },
    { "VRSQRT", 370, CPI_COP_DEFAULT },
    { "VILWR", 371, CPI_COP_DEFAULT },
    { "VRINIT", 372, CPI_COP_DEFAULT },
    { "VADDAbc.3", 373, CPI_COP_DEFAULT },
    { "VSUBAbc.3", 374, CPI_COP_DEFAULT },
    { "VMADDAbc.3", 375, CPI_COP_DEFAULT },
    { "VMSUBAbc.3", 376, CPI_COP_DEFAULT },
    { "VITOF15", 377, CPI_COP_DEFAULT },
    { "VFTIO15", 378, CPI_COP_DEFAULT },
    { "VMULAbc.3", 379, CPI_COP_DEFAULT },
    { "VCLIP", 380, CPI_COP_DEFAULT },
    { "VMADDAi", 381, CPI_COP_DEFAULT },
    { "VMSUBAi", 382, CPI_COP_DEFAULT },
    { "VNOP", 383, CPI_COP_DEFAULT },
    { "VSQD", 384, CPI_COP_DEFAULT },
    { "VWAITQ", 385, CPI_COP_DEFAULT },
    { "VISWR", 386, CPI_COP_DEFAULT },
    { "VRXOR", 387, CPI_COP_DEFAULT },
    { "VADDbc.0", 268, CPI_COP_DEFAULT },
    { "VADDbc.1", 269, CPI_COP_DEFAULT },
    { "VADDbc.2", 270, CPI_COP_DEFAULT },
    { "VADDbc.3", 271, CPI_COP_DEFAULT },
    { "VSUBbc.0", 272, CPI_COP_DEFAULT },
    { "VSUBbc.1", 273, CPI_COP_DEFAULT },
    { "VSUBbc.2", 274, CPI_COP_DEFAULT },
    { "VSUBbc.3", 275, CPI_COP_DEFAULT },
    { "VMADDbc.0", 276, CPI_COP_DEFAULT },
    { "VMADDbc.1", 277, CPI_COP_DEFAULT },
    { "VMADDbc.2", 278, CPI_COP_DEFAULT },
    { "VMADDbc.3", 279, CPI_COP_DEFAULT },
    { "VMSUBbc.0", 280, CPI_COP_DEFAULT },
    { "VMSUBbc.1", 281, CPI_COP_DEFAULT },
    { "VMSUBbc.2", 282, CPI_COP_DEFAULT },
    { "VMSUBbc.3", 283, CPI_COP_DEFAULT },
    { "VMAXbc.0", 284, CPI_COP_DEFAULT },
    { "VMAXbc.1", 285, CPI_COP_DEFAULT },
    { "VMAXbc.2", 286, CPI_COP_DEFAULT },
    { "VMAXbc.3", 287, CPI_COP_DEFAULT },
    { "VMINIbc.0", 288, CPI_COP_DEFAULT },
    { "VMINIbc.1", 289, CPI_COP_DEFAULT },
    { "VMINIbc.2", 290, CPI_COP_DEFAULT },
    { "VMINIbc.3", 291, CPI_COP_DEFAULT },
    { "VMULbc.0", 292, CPI_COP_DEFAULT },
    { "VMULbc.1", 293, CPI_COP_DEFAULT },
    { "VMULbc.2", 294, CPI_COP_DEFAULT },
    { "VMULbc.3", 295, CPI_COP_DEFAULT },
    { "VMULq", 296, CPI_COP_DEFAULT },
    { "VMAXi", 297, CPI_COP_DEFAULT },
    { "VMULi", 298, CPI_COP_DEFAULT },
    { "VMINIi", 299, CPI_COP_DEFAULT },
    { "VADDq", 300, CPI_COP_DEFAULT },
    { "VMADDq", 301, CPI_COP_DEFAULT },
    { "VADDi", 302, CPI_COP_DEFAULT },
    { "VMADDi", 303, CPI_COP_DEFAULT },
    { "VSUBq", 304, CPI_COP_DEFAULT },
    { "VMSUBq", 305, CPI_COP_DEFAULT },
    { "VSUBi", 306, CPI_COP_DEFAULT },
    { "VMSUBi", 307, CPI_COP_DEFAULT },
    { "VADD", 308, CPI_COP_DEFAULT },
    { "VMADD", 309, CPI_COP_DEFAULT },
    { "VMUL", 310, CPI_COP_DEFAULT },
    { "VMAX", 311, CPI_COP_DEFAULT },
    { "VSUB", 312, CPI_COP_DEFAULT },
    { "VMSUB", 313, CPI_COP_DEFAULT },
    { "VOPMSUB", 314, CPI_COP_DEFAULT },
    { "VMINI", 315, CPI_COP_DEFAULT },
    { "VIADD", 316, CPI_COP_DEFAULT },
    { "VISUB", 317, CPI_COP_DEFAULT },
    { "VIADDI", 318, CPI_COP_DEFAULT },
    { "VIAND", 319, CPI_COP_DEFAULT },
    { "VIOR", 320, CPI_COP_DEFAULT },
    { "VCALLMS", 321, CPI_COP_DEFAULT },
    { "VCALLMSR", 322, CPI_COP_DEFAULT },
    { "J", 1, CPI_R5900_BRANCH },
    { "JAL", 2, CPI_R5900_BRANCH },
    { "BEQ", 3, CPI_R5900_BRANCH },
    { "BNE", 4, CPI_R5900_BRANCH },
    { "BLEZ", 5, CPI_R5900_BRANCH },
    { "BGTZ", 6, CPI_R5900_BRANCH },
    { "ADDI", 7, CPI_R5900_DEFAULT },
    { "ADDIU", 8, CPI_R5900_DEFAULT },
    { "SLTI", 9, CPI_R5900_DEFAULT },
    { "SLTIU", 10, CPI_R5900_DEFAULT },
    { "ANDI", 11, CPI_R5900_DEFAULT },
    { "ORI", 12, CPI_R5900_DEFAULT },
    { "XORI", 13, CPI_R5900_DEFAULT },
    { "LUI", 14, CPI_R5900_DEFAULT },
    { "BEQL", 15, CPI_R5900_BRANCH },
    { "BNEL", 16, CPI_R5900_BRANCH },
    { "BLEZL", 17, CPI_R5900_BRANCH },
    { "BGTZL", 18, CPI_R5900_BRANCH },
    { "DADDI", 19, CPI_R5900_DEFAULT },
    { "DADDIU", 20, CPI_R5900_DEFAULT },
    { "LDL", 21, CPI_R5900_LOAD },
    { "LDR", 22, CPI_R5900_LOAD },
    { "LQ", 23, CPI_R5900_LOAD },
    { "SQ", 24, CPI_R5900_STORE },
    { "LB", 25, CPI_R5900_LOAD },
    { "LH", 26, CPI_R5900_LOAD },
    { "LWL", 27, CPI_R5900_LOAD },
    { "LW", 28, CPI_R5900_LOAD },
    { "LBU", 29, CPI_R5900_LOAD },
    { "LHU", 30, CPI_R5900_LOAD },
    { "LWR", 31, CPI_R5900_LOAD },
    { "LWU", 32, CPI_R5900_LOAD },
    { "SB", 33, CPI_R5900_STORE },
    { "SH", 34, CPI_R5900_STORE },
    { "SWL", 36, CPI_R5900_STORE },
    { "SDL", 37, CPI_R5900_STORE },
    { "SDR", 38, CPI_R5900_STORE },
    { "SWR", 39, CPI_R5900_STORE },
    { "CACHE", 40, CPI_R5900_DEFAULT },
    { "LWC1", 41, CPI_R5900_LOAD },
    { "PREF", 42, CPI_R5900_DEFAULT },
    { "LQC2", 43, CPI_R5900_LOAD },
    { "LD", 44, CPI_R5900_LOAD },
    { "SWC1", 45, CPI_R5900_STORE },
    { "SQC2", 46, CPI_R5900_STORE },
    { "SD", 47, CPI_R5900_STORE },
};
