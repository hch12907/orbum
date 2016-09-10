#include "stdafx.h"

#include "Common/Global/Globals.h"

#include "Common/PS2Resources/EE/EE_t.h"
#include "Common/PS2Resources/Types/StorageObject/StorageObject_t.h"
#include "Common/PS2Resources/Types/DeadStorageObject/DeadStorageObject_t.h"
#include "Common/PS2Resources/Types/BusErrorStorageObject/BusErrorStorageObject_t.h"
#include "Common/PS2Resources/EE/Types/EERegisters_t.h"
#include "Common/PS2Resources/EE/EECore/EECore_t.h"

EE_t::EE_t(const PS2Resources_t* const PS2Resources) :
	PS2ResourcesSubobject(PS2Resources),
	EECore(std::make_shared<EECore_t>(getRootResources())),

	// Registers.
	EE_REGISTER_T0_COUNT(std::make_shared<StorageObject_t>(0x10, "TIMER: T0_COUNT", 0x10000000)),
	EE_REGISTER_T0_MODE(std::make_shared<StorageObject_t>(0x10, "TIMER: T0_MODE", 0x10000010)),
	EE_REGISTER_T0_COMP(std::make_shared<StorageObject_t>(0x10, "TIMER: T0_COMP", 0x10000020)),
	EE_REGISTER_T0_HOLD(std::make_shared<StorageObject_t>(0x10, "TIMER: T0_HOLD", 0x10000030)),
	EE_REGISTER_TIMER_0040(std::make_shared<DeadStorageObject_t>(0x7C0, "TIMER: TIMER_0040 (reserved)", 0x10000040)),
	EE_REGISTER_T1_COUNT(std::make_shared<StorageObject_t>(0x10, "TIMER: T1_COUNT", 0x10000800)),
	EE_REGISTER_T1_MODE(std::make_shared<StorageObject_t>(0x10, "TIMER: T1_MODE", 0x10000810)),
	EE_REGISTER_T1_COMP(std::make_shared<StorageObject_t>(0x10, "TIMER: T1_COMP", 0x10000820)),
	EE_REGISTER_T1_HOLD(std::make_shared<StorageObject_t>(0x10, "TIMER: T1_HOLD", 0x10000830)),
	EE_REGISTER_TIMER_0840(std::make_shared<DeadStorageObject_t>(0x7C0, "TIMER: TIMER_0840 (reserved)", 0x10000840)),
	EE_REGISTER_T2_COUNT(std::make_shared<StorageObject_t>(0x10, "TIMER: T2_COUNT", 0x10001000)),
	EE_REGISTER_T2_MODE(std::make_shared<StorageObject_t>(0x10, "TIMER: T2_MODE", 0x10001010)),
	EE_REGISTER_T2_COMP(std::make_shared<StorageObject_t>(0x10, "TIMER: T2_COMP", 0x10001020)),
	EE_REGISTER_TIMER_1030(std::make_shared<DeadStorageObject_t>(0x7D0, "TIMER: TIMER_1030 (reserved)", 0x10001030)),
	EE_REGISTER_T3_COUNT(std::make_shared<StorageObject_t>(0x10, "TIMER: T3_COUNT", 0x10001800)),
	EE_REGISTER_T3_MODE(std::make_shared<StorageObject_t>(0x10, "TIMER: T3_MODE", 0x10001810)),
	EE_REGISTER_T3_COMP(std::make_shared<StorageObject_t>(0x10, "TIMER: T3_COMP", 0x10001820)),
	EE_REGISTER_TIMER_1830(std::make_shared<DeadStorageObject_t>(0x7D0, "TIMER: TIMER_1830 (reserved)", 0x10001830)),
	EE_REGISTER_IPU_CMD(std::make_shared<StorageObject_t>(0x10, "IPU: IPU_CMD", 0x10002000)),
	EE_REGISTER_IPU_CTRL(std::make_shared<StorageObject_t>(0x10, "IPU: IPU_CTRL", 0x10002010)),
	EE_REGISTER_IPU_BP(std::make_shared<StorageObject_t>(0x10, "IPU: IPU_BP", 0x10002020)),
	EE_REGISTER_IPU_TOP(std::make_shared<StorageObject_t>(0x10, "IPU: IPU_TOP", 0x10002030)),
	EE_REGISTER_IPU_2040(std::make_shared<DeadStorageObject_t>(0xFC0, "IPU: IPU_2040 (reserved)", 0x10002040)),
	EE_REGISTER_GIF_CTRL(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_CTRL", 0x10003000)),
	EE_REGISTER_GIF_MODE(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_MODE", 0x10003010)),
	EE_REGISTER_GIF_STAT(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_STAT", 0x10003020)),
	EE_REGISTER_GIF_3030(std::make_shared<DeadStorageObject_t>(0x10, "GIF: GIF_3030 (reserved)", 0x10003030)),
	EE_REGISTER_GIF_TAG0(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_TAG0", 0x10003040)),
	EE_REGISTER_GIF_TAG1(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_TAG1", 0x10003050)),
	EE_REGISTER_GIF_TAG2(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_TAG2", 0x10003060)),
	EE_REGISTER_GIF_TAG3(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_TAG3", 0x10003070)),
	EE_REGISTER_GIF_CNT(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_CNT", 0x10003080)),
	EE_REGISTER_GIF_P3CNT(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_P3CNT", 0x10003090)),
	EE_REGISTER_GIF_P3TAG(std::make_shared<StorageObject_t>(0x10, "GIF: GIF_P3TAG", 0x100030a0)),
	EE_REGISTER_GIF_30b0(std::make_shared<DeadStorageObject_t>(0x750, "GIF: GIF_30b0 (reserved)", 0x100030b0)),
	EE_REGISTER_VIF0_STAT(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_STAT", 0x10003800)),
	EE_REGISTER_VIF0_FBRST(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_FBRST", 0x10003810)),
	EE_REGISTER_VIF0_ERR(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_ERR", 0x10003820)),
	EE_REGISTER_VIF0_MARK(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_MARK", 0x10003830)),
	EE_REGISTER_VIF0_CYCLE(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_CYCLE", 0x10003840)),
	EE_REGISTER_VIF0_MODE(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_MODE", 0x10003850)),
	EE_REGISTER_VIF0_NUM(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_NUM", 0x10003860)),
	EE_REGISTER_VIF0_MASK(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_MASK", 0x10003870)),
	EE_REGISTER_VIF0_CODE(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_CODE", 0x10003880)),
	EE_REGISTER_VIF0_ITOPS(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_ITOPS", 0x10003890)),
	EE_REGISTER_VIF0_38a0(std::make_shared<DeadStorageObject_t>(0x10, "VIF0: VIF0_38a0 (reserved)", 0x100038a0)),
	EE_REGISTER_VIF0_38b0(std::make_shared<DeadStorageObject_t>(0x10, "VIF0: VIF0_38b0 (reserved)", 0x100038b0)),
	EE_REGISTER_VIF0_38c0(std::make_shared<DeadStorageObject_t>(0x10, "VIF0: VIF0_38c0 (reserved)", 0x100038c0)),
	EE_REGISTER_VIF0_ITOP(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_ITOP", 0x100038d0)),
	EE_REGISTER_VIF0_38e0(std::make_shared<DeadStorageObject_t>(0x10, "VIF0: VIF0_38e0 (reserved)", 0x100038e0)),
	EE_REGISTER_VIF0_38f0(std::make_shared<DeadStorageObject_t>(0x10, "VIF0: VIF0_38f0 (reserved)", 0x100038f0)),
	EE_REGISTER_VIF0_R0(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_R0", 0x10003900)),
	EE_REGISTER_VIF0_R1(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_R1", 0x10003910)),
	EE_REGISTER_VIF0_R2(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_R2", 0x10003920)),
	EE_REGISTER_VIF0_R3(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_R3", 0x10003930)),
	EE_REGISTER_VIF0_C0(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_C0", 0x10003940)),
	EE_REGISTER_VIF0_C1(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_C1", 0x10003950)),
	EE_REGISTER_VIF0_C2(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_C2", 0x10003960)),
	EE_REGISTER_VIF0_C3(std::make_shared<StorageObject_t>(0x10, "VIF0: VIF0_C3", 0x10003970)),
	EE_REGISTER_VIF0_3980(std::make_shared<DeadStorageObject_t>(0x280, "VIF0: VIF0_3980 (reserved)", 0x10003980)),
	EE_REGISTER_VIF1_STAT(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_STAT", 0x10003c00)),
	EE_REGISTER_VIF1_FBRST(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_FBRST", 0x10003c10)),
	EE_REGISTER_VIF1_ERR(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_ERR", 0x10003c20)),
	EE_REGISTER_VIF1_MARK(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_MARK", 0x10003c30)),
	EE_REGISTER_VIF1_CYCLE(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_CYCLE", 0x10003c40)),
	EE_REGISTER_VIF1_MODE(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_MODE", 0x10003c50)),
	EE_REGISTER_VIF1_NUM(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_NUM", 0x10003c60)),
	EE_REGISTER_VIF1_MASK(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_MASK", 0x10003c70)),
	EE_REGISTER_VIF1_CODE(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_CODE", 0x10003c80)),
	EE_REGISTER_VIF1_ITOPS(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_ITOPS", 0x10003c90)),
	EE_REGISTER_VIF1_BASE(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_BASE", 0x10003ca0)),
	EE_REGISTER_VIF1_OFST(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_OFST", 0x10003cb0)),
	EE_REGISTER_VIF1_TOPS(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_TOPS", 0x10003cc0)),
	EE_REGISTER_VIF1_ITOP(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_ITOP", 0x10003cd0)),
	EE_REGISTER_VIF1_TOP(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_TOP", 0x10003ce0)),
	EE_REGISTER_VIF1_3cf0(std::make_shared<DeadStorageObject_t>(0x10, "VIF1: VIF1_3cf0 (reserved)", 0x10003cf0)),
	EE_REGISTER_VIF1_R0(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_R0", 0x10003d00)),
	EE_REGISTER_VIF1_R1(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_R1", 0x10003d10)),
	EE_REGISTER_VIF1_R2(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_R2", 0x10003d20)),
	EE_REGISTER_VIF1_R3(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_R3", 0x10003d30)),
	EE_REGISTER_VIF1_C0(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_C0", 0x10003d40)),
	EE_REGISTER_VIF1_C1(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_C1", 0x10003d50)),
	EE_REGISTER_VIF1_C2(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_C2", 0x10003d60)),
	EE_REGISTER_VIF1_C3(std::make_shared<StorageObject_t>(0x10, "VIF1: VIF1_C3", 0x10003d70)),
	EE_REGISTER_VIF1_3d80(std::make_shared<DeadStorageObject_t>(0x280, "VIF1: VIF1_3d80 (reserved)", 0x10003d80)),
	EE_REGISTER_VIF0_FIFO(std::make_shared<StorageObject_t>(0x10, "FIFO: VIF0_FIFO", 0x10004000)),
	EE_REGISTER_VIF1_FIFO(std::make_shared<StorageObject_t>(0x10, "FIFO: VIF1_FIFO", 0x10005000)),
	EE_REGISTER_GIF_FIFO(std::make_shared<StorageObject_t>(0x10, "FIFO: GIF_FIFO", 0x10006000)),
	EE_REGISTER_IPU_out_FIFO(std::make_shared<StorageObject_t>(0x10, "FIFO: IPU_out_FIFO", 0x10007000)),
	EE_REGISTER_IPU_in_FIFO(std::make_shared<StorageObject_t>(0x10, "FIFO: IPU_in_FIFO", 0x10007010)),
	EE_REGISTER_D0_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D0_CHCR", 0x10008000)),
	EE_REGISTER_D0_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D0_MADR", 0x10008010)),
	EE_REGISTER_D0_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D0_QWC", 0x10008020)),
	EE_REGISTER_D0_TADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D0_TADR", 0x10008030)),
	EE_REGISTER_D0_ASR0(std::make_shared<StorageObject_t>(0x10, "DMAC: D0_ASR0", 0x10008040)),
	EE_REGISTER_D0_ASR1(std::make_shared<StorageObject_t>(0x10, "DMAC: D0_ASR1", 0x10008050)),
	EE_REGISTER_DMAC_8060(std::make_shared<DeadStorageObject_t>(0xFA0, "DMAC: DMAC_8060 (reserved)", 0x10008060)),
	EE_REGISTER_D1_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D1_CHCR", 0x10009000)),
	EE_REGISTER_D1_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D1_MADR", 0x10009010)),
	EE_REGISTER_D1_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D1_QWC", 0x10009020)),
	EE_REGISTER_D1_TADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D1_TADR", 0x10009030)),
	EE_REGISTER_D1_ASR0(std::make_shared<StorageObject_t>(0x10, "DMAC: D1_ASR0", 0x10009040)),
	EE_REGISTER_D1_ASR1(std::make_shared<StorageObject_t>(0x10, "DMAC: D1_ASR1", 0x10009050)),
	EE_REGISTER_DMAC_9060(std::make_shared<DeadStorageObject_t>(0xFA0, "DMAC: DMAC_9060 (reserved)", 0x10009060)),
	EE_REGISTER_D2_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D2_CHCR", 0x1000a000)),
	EE_REGISTER_D2_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D2_MADR", 0x1000a010)),
	EE_REGISTER_D2_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D2_QWC", 0x1000a020)),
	EE_REGISTER_D2_TADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D2_TADR", 0x1000a030)),
	EE_REGISTER_D2_ASR0(std::make_shared<StorageObject_t>(0x10, "DMAC: D2_ASR0", 0x1000a040)),
	EE_REGISTER_D2_ASR1(std::make_shared<StorageObject_t>(0x10, "DMAC: D2_ASR1", 0x1000a050)),
	EE_REGISTER_DMAC_a060(std::make_shared<DeadStorageObject_t>(0xFA0, "DMAC: DMAC_a060 (reserved)", 0x1000a060)),
	EE_REGISTER_D3_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D3_CHCR", 0x1000b000)),
	EE_REGISTER_D3_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D3_MADR", 0x1000b010)),
	EE_REGISTER_D3_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D3_QWC", 0x1000b020)),
	EE_REGISTER_DMAC_b030(std::make_shared<DeadStorageObject_t>(0x3D0, "DMAC: DMAC_b030 (reserved)", 0x1000b030)),
	EE_REGISTER_D4_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D4_CHCR", 0x1000b400)),
	EE_REGISTER_D4_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D4_MADR", 0x1000b410)),
	EE_REGISTER_D4_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D4_QWC", 0x1000b420)),
	EE_REGISTER_D4_TADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D4_TADR", 0x1000b430)),
	EE_REGISTER_DMAC_b440(std::make_shared<DeadStorageObject_t>(0xBC0, "DMAC: DMAC_b440 (reserved)", 0x1000b440)),
	EE_REGISTER_D5_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D5_CHCR", 0x1000c000)),
	EE_REGISTER_D5_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D5_MADR", 0x1000c010)),
	EE_REGISTER_D5_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D5_QWC", 0x1000c020)),
	EE_REGISTER_DMAC_c030(std::make_shared<DeadStorageObject_t>(0x3D0, "DMAC: DMAC_c030 (reserved)", 0x1000c030)),
	EE_REGISTER_D6_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D6_CHCR", 0x1000c400)),
	EE_REGISTER_D6_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D6_MADR", 0x1000c410)),
	EE_REGISTER_D6_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D6_QWC", 0x1000c420)),
	EE_REGISTER_D6_TADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D6_TADR", 0x1000c430)),
	EE_REGISTER_DMAC_c440(std::make_shared<DeadStorageObject_t>(0x3C0, "DMAC: DMAC_c440 (reserved)", 0x1000c440)),
	EE_REGISTER_D7_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D7_CHCR", 0x1000c800)),
	EE_REGISTER_D7_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D7_MADR", 0x1000c810)),
	EE_REGISTER_D7_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D7_QWC", 0x1000c820)),
	EE_REGISTER_DMAC_c830(std::make_shared<DeadStorageObject_t>(0x7D0, "DMAC: DMAC_c830 (reserved)", 0x1000c830)),
	EE_REGISTER_D8_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D8_CHCR", 0x1000d000)),
	EE_REGISTER_D8_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D8_MADR", 0x1000d010)),
	EE_REGISTER_D8_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D8_QWC", 0x1000d020)),
	EE_REGISTER_DMAC_d030(std::make_shared<DeadStorageObject_t>(0x50, "DMAC: DMAC_d030 (reserved)", 0x1000d030)),
	EE_REGISTER_D8_SADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D8_SADR", 0x1000d080)),
	EE_REGISTER_DMAC_d090(std::make_shared<DeadStorageObject_t>(0x370, "DMAC: DMAC_d090 (reserved)", 0x1000d090)),
	EE_REGISTER_D9_CHCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D9_CHCR", 0x1000d400)),
	EE_REGISTER_D9_MADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D9_MADR", 0x1000d410)),
	EE_REGISTER_D9_QWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D9_QWC", 0x1000d420)),
	EE_REGISTER_D9_TADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D9_TADR", 0x1000d430)),
	EE_REGISTER_DMAC_d440(std::make_shared<DeadStorageObject_t>(0x40, "DMAC: DMAC_d440 (reserved)", 0x1000d440)),
	EE_REGISTER_D9_SADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D9_SADR", 0x1000d480)),
	EE_REGISTER_DMAC_d490(std::make_shared<DeadStorageObject_t>(0xB70, "DMAC: DMAC_d490 (reserved)", 0x1000d490)),
	EE_REGISTER_D_CTRL(std::make_shared<StorageObject_t>(0x10, "DMAC: D_CTRL", 0x1000e000)),
	EE_REGISTER_D_STAT(std::make_shared<StorageObject_t>(0x10, "DMAC: D_STAT", 0x1000e010)),
	EE_REGISTER_D_PCR(std::make_shared<StorageObject_t>(0x10, "DMAC: D_PCR", 0x1000e020)),
	EE_REGISTER_D_SQWC(std::make_shared<StorageObject_t>(0x10, "DMAC: D_SQWC", 0x1000e030)),
	EE_REGISTER_D_RBSR(std::make_shared<StorageObject_t>(0x10, "DMAC: D_RBSR", 0x1000e040)),
	EE_REGISTER_D_RBOR(std::make_shared<StorageObject_t>(0x10, "DMAC: D_RBOR", 0x1000e050)),
	EE_REGISTER_D_STADR(std::make_shared<StorageObject_t>(0x10, "DMAC: D_STADR", 0x1000e060)),
	EE_REGISTER_DMAC_e070(std::make_shared<DeadStorageObject_t>(0xF90, "DMAC: DMAC_e070 (reserved)", 0x1000e070)),
	EE_REGISTER_I_STAT(std::make_shared<StorageObject_t>(0x10, "INTC: I_STAT", 0x1000f000)),
	EE_REGISTER_I_MASK(std::make_shared<StorageObject_t>(0x10, "INTC: I_MASK", 0x1000f010)),
	EE_REGISTER_INTC_f020(std::make_shared<DeadStorageObject_t>(0xE0, "INTC: INTC_f020 (reserved)", 0x1000f020)),
	EE_REGISTER_SIO(std::make_shared<EERegisterSIO_t>()),
	EE_REGISTER_SB_SMFLG(std::make_shared<StorageObject_t>(0x10, "SIF: SB_SMFLG", 0x1000f230)),
	EE_REGISTER_F400(std::make_shared<StorageObject_t>(0x10, "Undocumented: F400", 0x1000f400)),
	EE_REGISTER_F410(std::make_shared<DeadStorageObject_t>(0x10, "Undocumented: F410", 0x1000f410)),
	EE_REGISTER_F420(std::make_shared<StorageObject_t>(0x10, "Undocumented: F420", 0x1000f420)),
	EE_REGISTER_MCH(std::make_shared<EERegisterMCH_t>()),
	EE_REGISTER_F450(std::make_shared<StorageObject_t>(0xB0, "Undocumented: F450", 0x1000f450)),
	EE_REGISTER_F500(std::make_shared<StorageObject_t>(0x20, "Undocumented: F500", 0x1000f500)),
	EE_REGISTER_D_ENABLER(std::make_shared<StorageObject_t>(0x10, "DMAC: D_ENABLER", 0x1000f520)),
	EE_REGISTER_F530(std::make_shared<DeadStorageObject_t>(0x60, "DMAC: F530 (reserved)", 0x1000f530)),
	EE_REGISTER_D_ENABLEW(std::make_shared<StorageObject_t>(0x10, "DMAC: D_ENABLEW", 0x1000f590)),
	EE_REGISTER_F5A0(std::make_shared<DeadStorageObject_t>(0x60, "DMAC: F5A0 (reserved)", 0x1000f5A0)),
	VU_MEM_VU0Micro0(std::make_shared<StorageObject_t>(0x1000, "VU0: VU0Micro0", 0x11000000)),
	VU_MEM_VU0Mem0(std::make_shared<StorageObject_t>(0x1000, "VU0: VU0Mem0", 0x11004000)),
	VU_MEM_VU1Micro1(std::make_shared<StorageObject_t>(0x4000, "VU1: VU1Micro1", 0x11008000)),
	VU_MEM_VU1Mem1(std::make_shared<StorageObject_t>(0x4000, "VU1: VU1Mem1", 0x1100c000)),
	VU_MEM_VU1BusErr(std::make_shared<BusErrorStorageObject_t>(0x4000, "VU1: VU1BusErr", 0x11010000))
{
}
