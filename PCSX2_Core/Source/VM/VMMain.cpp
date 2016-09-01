#include "stdafx.h"

#include <memory>
#include <fstream>

#include "VM/VMMain.h"
#include "VM/VMMMUHandler/VMMMUHandler.h"
#include "VM/ExecutionCore/Interpreter/Interpreter.h"


VMMain::VMMain(ExecutionCoreType executionCoreType, const std::string & bootROMPath) 
	: mStatus(VMMain::VMStatus::CREATED),
	mExecutionCoreType(executionCoreType),
	mVMMMUComponent(std::make_unique<VMMMUHandler>(this)),
	mBootROMPath(bootROMPath)
{
	// Initialise everything.
	Reset();
}

void VMMain::LoadExecutable(std::string&& excutablePath) const
{
	// TODO: Implement.
}

void VMMain::Reset()
{
	// Initalise VM.
	initaliseResources();
	initaliseExecutionCore();
	initalisePS2PhysicalMemoryMap();

	// Initiaise BIOS (BootROM).
	initaliseBootROM();
	
	// Initalise the execution core.
	mExecutionCoreComponent->initalise();
}

void VMMain::Run()
{
	// Set to running.
	mStatus = VMStatus::RUNNING;

	// Run the VM.
	while (mStatus == VMStatus::RUNNING)
	{
		mExecutionCoreComponent->executionStep();
	}
}

void VMMain::Stop()
{
	// TODO: Implement.
}

VMMain::~VMMain()
{
}

const VMMain::VMStatus& VMMain::getStatus() const
{
	return mStatus;
}

const std::unique_ptr<PS2Resources_t>& VMMain::getResources() const
{
	return mPS2Resources;
}

const std::unique_ptr<VMMMUComponent>& VMMain::getMMU() const
{
	return mVMMMUComponent;
}

void VMMain::initaliseResources()
{
	// Any old resources are destroyed automatically through unique_ptr.
	mPS2Resources = std::make_unique<PS2Resources_t>();
}

void VMMain::initalisePS2PhysicalMemoryMap() const
{
	// Main memory 32MB
	getMMU()->mapMemory(getResources()->MainMemory);

	// Scratchpad memory 16KB
	getMMU()->mapMemory(getResources()->EE->EECore->ScratchpadMemory);

	// Boot ROM 4MB
	getMMU()->mapMemory(getResources()->BootROM);

	// EE Registers.
	getMMU()->mapMemory(getResources()->EE->EE_REGISTER_SIO);
	getMMU()->mapMemory(getResources()->EE->EE_REGISTER_F410);
}

void VMMain::initaliseExecutionCore()
{
	switch (mExecutionCoreType)
	{
	case ExecutionCoreType::INTERPRETER:
	{
		mExecutionCoreComponent = std::make_unique<Interpreter>(this);
		break;
	}
	case ExecutionCoreType::RECOMPILER:
	{
		// TODO: Implement.
		break;
	}
	}
}

void VMMain::initaliseBootROM() const
{
	char * memoryBase = reinterpret_cast<char*>(getResources()->BootROM->getClientMemoryAddress());
	std::ifstream file(mBootROMPath, std::ifstream::binary);
	if (file.fail()) 
		throw std::runtime_error("initaliseBootROM(): tried to open BIOS file, but it failed! Check file exists and has read permissions.");
	file.seekg(std::ifstream::beg);
	file.read(memoryBase, PS2Constants::BootROM::SIZE_BOOT_ROM);
}
