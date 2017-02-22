#include "stdafx.h"

#include "Common/Global/Globals.h"
#include "Common/Types/Registers/Register128_t.h"
#include "Common/Types/Registers/MIPS/PCRegister32_t.h"
#include "Common/Tables/EECoreSyscallTable/EECoreSyscallTable.h"

#include "VM/Systems/EE/EECoreInterpreter/EECoreInterpreter_s.h"

#include "Resources/EE/EECore/EECore_t.h"
#include "Resources/EE/EECore/Types/EECoreR5900_t.h"
#include "Resources/EE/EECore/Types/EECoreCOP0_t.h"
#include "Resources/EE/EECore/Types/EECoreCOP0Registers_t.h"
#include "Resources/EE/EECore/Types/EECoreException_t.h"

void EECoreInterpreter_s::BREAK()
{
	// EXCEPTION(BREAKPOINT)
	handleException(EECoreException_t::EX_BREAK);
}

void EECoreInterpreter_s::SYSCALL()
{
#if DEBUG_SYSCALL_LOG
	// Debug print the syscall mnemonic.
	// The convention is to store the syscall number in register $v1 ($3), then use the syscall instruction (the 'code' field within the syscall instruction is apparently unused).
	// When the syscall number is loaded into $v1, it is done so through
	//   ADDIU $v1, $0, number.
	// If the 16-bit 'syscall number' above has the sign bit set (negative), the EE OS will first make it unsigned then call the handler with the (i) prefix... TODO: not sure what the differnece is.
	// The EE OS only defines handlers for syscall numbers 0 -> 127 (128 total). 
	u8 index = mEECore->R5900->GPR[3]->readByte(EE, 0);
	log(Debug, "EECore Syscall, number %d (%s).", index, EECoreSyscallTable::getSyscallMnemonic(index));
#endif

	// EXCEPTION(SYSCALL)
	handleException(EECoreException_t::EX_SYSTEMCALL);
}

void EECoreInterpreter_s::TEQ()
{
	// TRAP_EXCEPTION(Rs == Rt).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getRRs()];
	auto& source2Reg = mEECore->R5900->GPR[mInstruction.getRRt()];

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));
	auto source2Val = static_cast<s64>(source2Reg->readDword(EE, 0));

	if (source1Val == source2Val)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TEQI()
{
	// TRAP_EXCEPTION(Rs == imm).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getIRs()];
	auto imm = static_cast<s64>(mInstruction.getIImmS());

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));

	if (source1Val == imm)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TGE()
{
	// TRAP_EXCEPTION(Rs >= Rt).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getRRs()];
	auto& source2Reg = mEECore->R5900->GPR[mInstruction.getRRt()];

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));
	auto source2Val = static_cast<s64>(source2Reg->readDword(EE, 0));

	if (source1Val >= source2Val)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TGEI()
{
	// TRAP_EXCEPTION(Rs >= imm).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getIRs()];
	auto imm = static_cast<s64>(mInstruction.getIImmS());

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));

	if (source1Val >= imm)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TGEIU()
{
	// TRAP_EXCEPTION(Rs >= imm).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getIRs()];
	auto imm = static_cast<u64>(static_cast<s64>(mInstruction.getIImmS())); // Sign-extend first, then treat as unsigned. This is according to the docs.

	auto source1Val = static_cast<u64>(source1Reg->readDword(EE, 0));

	if (source1Val >= imm)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TGEU()
{
	// TRAP_EXCEPTION(Rs >= Rt).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getRRs()];
	auto& source2Reg = mEECore->R5900->GPR[mInstruction.getRRt()];

	auto source1Val = static_cast<u64>(source1Reg->readDword(EE, 0));
	auto source2Val = static_cast<u64>(source2Reg->readDword(EE, 0));

	if (source1Val >= source2Val)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TLT()
{
	// TRAP_EXCEPTION(Rs < Rt).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getRRs()];
	auto& source2Reg = mEECore->R5900->GPR[mInstruction.getRRt()];

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));
	auto source2Val = static_cast<s64>(source2Reg->readDword(EE, 0));

	if (source1Val < source2Val)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TLTI()
{
	// TRAP_EXCEPTION(Rs < imm).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getIRs()];
	auto imm = static_cast<s64>(mInstruction.getIImmS());

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));

	if (source1Val < imm)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TLTIU()
{
	// TRAP_EXCEPTION(Rs < imm).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getIRs()];
	auto imm = static_cast<u64>(static_cast<s64>(mInstruction.getIImmS())); // Sign-extend first, then treat as unsigned. This is according to the docs.

	auto source1Val = static_cast<u64>(source1Reg->readDword(EE, 0));

	if (source1Reg->readDword(EE, 0) < imm)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TLTU()
{
	// TRAP_EXCEPTION(Rs < Rt).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getRRs()];
	auto& source2Reg = mEECore->R5900->GPR[mInstruction.getRRt()];

	if (source1Reg->readDword(EE, 0) < source2Reg->readDword(EE, 0))
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TNE()
{
	// TRAP_EXCEPTION(Rs != Rt).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getRRs()];
	auto& source2Reg = mEECore->R5900->GPR[mInstruction.getRRt()];

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));
	auto source2Val = static_cast<s64>(source2Reg->readDword(EE, 0));

	if (source1Val != source2Val)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::TNEI()
{
	// TRAP_EXCEPTION(Rs != imm).
	auto& source1Reg = mEECore->R5900->GPR[mInstruction.getIRs()];
	auto imm = static_cast<s64>(mInstruction.getIImmS());

	auto source1Val = static_cast<s64>(source1Reg->readDword(EE, 0));

	if (source1Val != imm)
		handleException(EECoreException_t::EX_TRAP);
}

void EECoreInterpreter_s::ERET()
{
	// ERET is an outlier, where it does not cause a branch delay instruction to be executed - set the PC directly.
	// ERET(). No exceptions.
	if (mEECore->COP0->Status->getFieldValue(EECoreCOP0Register_Status_t::Fields::ERL) > 0)
	{
		const u32 pcValue = mEECore->COP0->ErrorEPC->readWord(EE);
		mEECore->R5900->PC->setPCValueAbsolute(pcValue);
		mEECore->COP0->Status->setFieldValue(EECoreCOP0Register_Status_t::Fields::ERL, 0);
	}
	else
	{
		const u32 pcValue = mEECore->COP0->EPC->readWord(EE);
		mEECore->R5900->PC->setPCValueAbsolute(pcValue);
		mEECore->COP0->Status->setFieldValue(EECoreCOP0Register_Status_t::Fields::EXL, 0);
	}
}