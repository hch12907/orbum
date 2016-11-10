#include "stdafx.h"

#include "Common/Global/Globals.h"

#include "PS2Resources/EE/EE_t.h"
#include "Common/Types/Memory/Memory_t.h"
#include "Common/Types/Memory/DeadMemory_t.h"
#include "Common/Types/Registers/Register32_t.h"
#include "Common/Types/PhysicalMMU/PhysicalMMU_t.h"
#include "PS2Resources/PS2Resources_t.h"
#include "PS2Resources/EE/Types/EERegisters_t.h"
#include "PS2Resources/EE/Types/BootROM_t.h"
#include "PS2Resources/EE/EECore/EECore_t.h"
#include "PS2Resources/EE/Timers/Timers_t.h"
#include "PS2Resources/EE/DMAC/DMAC_t.h"
#include "PS2Resources/EE/INTC/INTC_t.h"
#include "PS2Resources/EE/IPU/IPU_t.h"
#include "PS2Resources/EE/GIF/GIF_t.h"
#include "PS2Resources/EE/VPU/VPU_t.h"


EE_t::EE_t(const PS2Resources_t* const PS2Resources) :
	PS2ResourcesSubobject(PS2Resources),
	EECore(std::make_shared<EECore_t>(getRootResources())),
	DMAC(std::make_shared<DMAC_t>(getRootResources())),
	INTC(std::make_shared<INTC_t>(getRootResources())),
	Timers(std::make_shared<Timers_t>(getRootResources())),
	IPU(std::make_shared<IPU_t>(getRootResources())),
	GIF(std::make_shared<GIF_t>(getRootResources())),
	VPU(std::make_shared<VPU_t>(getRootResources())),
	PhysicalMMU(std::make_shared<PhysicalMMU_t>(Constants::SIZE_4GB, Constants::SIZE_4MB, Constants::SIZE_16B)),
	MainMemory(std::make_shared<Memory_t>(PS2Constants::EE::MainMemory::SIZE_MAIN_MEMORY, "Main Memory")),
	BootROM(std::make_shared<BootROM_t>()),
	UNKNOWN_1A000000(std::make_shared<DeadMemory_t>(0x10000, "Unknown: 1A00")),

	// Registers.
	EE_FIFO_VIF0(std::make_shared<Memory_t>(0x10, "FIFO: VIF0_FIFO")),
	EE_FIFO_VIF1(std::make_shared<Memory_t>(0x10, "FIFO: VIF1_FIFO")),
	EE_FIFO_GIF(std::make_shared<Memory_t>(0x10, "FIFO: GIF_FIFO")),
	EE_FIFO_IPU_out(std::make_shared<Memory_t>(0x10, "FIFO: IPU_out_FIFO")),
	EE_FIFO_IPU_in(std::make_shared<Memory_t>(0x10, "FIFO: IPU_in_FIFO")),

	EE_MEMORY_SIO(std::make_shared<EERegister_SIO_t>()),
	EE_REGISTER_SB_SMFLG(std::make_shared<Register32_t>()),
	EE_REGISTER_F400(std::make_shared<Register32_t>()),
	EE_MEMORY_F410(std::make_shared<DeadMemory_t>(0x04, "Undocumented: F410")),
	EE_REGISTER_F420(std::make_shared<Register32_t>()),
	EE_MEMORY_MCH(std::make_shared<EERegister_MCH_t>()),
	EE_MEMORY_F450(std::make_shared<Memory_t>(0xB0, "Undocumented: F450"))
{
}