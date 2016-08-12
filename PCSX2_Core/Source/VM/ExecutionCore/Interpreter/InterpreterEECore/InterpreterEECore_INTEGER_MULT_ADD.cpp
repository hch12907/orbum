#include "stdafx.h"

#include "Common/Global/Globals.h"

#include "VM/ExecutionCore/Interpreter/InterpreterEECore/InterpreterEECore.h"
#include "Common/PS2 Resources/PS2Resources_t.h"
#include "Common/Util/EECoreInstructionUtil/EECoreInstructionUtil.h"

/*
Integer Multiply-Add instruction family.
*/

void InterpreterEECore::MADD(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = (LO || HI) + SignExtend<s64>(Rs[SW] * Rt[SW])
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	s64 result = ((static_cast<u64>(PS2Resources->EE->EECore->R5900->LO->readWordS(0)) << 32) || static_cast<u64>(PS2Resources->EE->EECore->R5900->LO->readWordS(0))) + (source1Reg->readWordS(0) * source2Reg->readWordS(0));
	PS2Resources->EE->EECore->R5900->LO->writeDwordS(0, static_cast<s64>(static_cast<s32>(result & 0xFFFFFFFF)));
	PS2Resources->EE->EECore->R5900->HI->writeDwordS(0, static_cast<s64>(static_cast<s32>(result >> 32)));
	destReg->writeDwordS(0, static_cast<s64>(static_cast<s32>(result & 0xFFFFFFFF)));
}

void InterpreterEECore::MADD1(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// Pipeline 1 instruction - delegate to normal instruction.
	MADD(instruction, PS2Resources);
}

void InterpreterEECore::MADDU(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = (LO || HI) + SignExtend<u64>(Rs[UW] * Rt[UW])
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	u64 result = ((static_cast<u64>(PS2Resources->EE->EECore->R5900->LO->readWordU(0)) << 32) || static_cast<u64>(PS2Resources->EE->EECore->R5900->LO->readWordU(0))) + (source1Reg->readWordU(0) * source2Reg->readWordU(0));
	PS2Resources->EE->EECore->R5900->LO->writeDwordS(0, static_cast<u64>(static_cast<u32>(result & 0xFFFFFFFF)));
	PS2Resources->EE->EECore->R5900->HI->writeDwordS(0, static_cast<u64>(static_cast<u32>(result >> 32)));
	destReg->writeDwordS(0, static_cast<u64>(static_cast<u32>(result & 0xFFFFFFFF)));
}

void InterpreterEECore::MADDU1(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// Pipeline 1 instruction - delegate to normal instruction.
	MADDU(instruction, PS2Resources);
}

void InterpreterEECore::PHMADH(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd)[i] = SignExtend<s32>(Rs[SH] * Rt[SH])[i] + SignExtend<s32>(Rs[SH] * Rt[SH])[i + 1]
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_HWORDS_IN_QWORD; i += 2)
	{
		s32 result = (source1Reg->readHwordS(i + 1) * source2Reg->readHwordS(i + 1)) + (source1Reg->readHwordS(i) * source2Reg->readHwordS(i));

		// Store in LO on i % 4 == 0 (every second loop), else store in HI, at word indexes 0 and 2.
		if (i % 4 == 0)
			PS2Resources->EE->EECore->R5900->LO->writeWordS((i / 4) * 2, result);
		else
			PS2Resources->EE->EECore->R5900->HI->writeWordS(((i - 2) / 4) * 2, result);

		// Set Rd (always done on each loop).
		destReg->writeWordS(i / 2, result);
	}
}

void InterpreterEECore::PHMSBH(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd)[i] = SignExtend<s32>(Rs[SH] * Rt[SH])[i] - SignExtend<s32>(Rs[SH] * Rt[SH])[i + 1]
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_HWORDS_IN_QWORD; i += 2)
	{
		s32 result = (source1Reg->readHwordS(i + 1) * source2Reg->readHwordS(i + 1)) - (source1Reg->readHwordS(i) * source2Reg->readHwordS(i));

		// Store in LO on i % 4 == 0 (every second loop), else store in HI, at word indexes 0 and 2.
		if (i % 4 == 0)
			PS2Resources->EE->EECore->R5900->LO->writeWordS((i / 4) * 2, result);
		else
			PS2Resources->EE->EECore->R5900->HI->writeWordS(((i - 2) / 4) * 2, result);

		// Set Rd (always done on each loop).
		destReg->writeWordS(i / 2, result);
	}
}

void InterpreterEECore::PMADDH(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = SignExtend<s32>(Rs[SH] * Rt[SH] + (HI,LO)[i]) (varying indexes - see EE Core Instruction Manual page 216).
	// (HI,LO)[i] = "C" in comments written below.
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()]; // "A"
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()]; // "B"
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_HWORDS_IN_QWORD; i++)
	{
		s32 result = source1Reg->readHwordS(i) * source2Reg->readHwordS(i);

		if ((i / 2) % 2 == 0)
		{
			auto index = ((i / 2 > 0) ? i - 2 : i); // A0xB0 + C0, A1xB1 + C1, A4xB4 + C4, A5xB5 + C5. Array ternary operator is to select the correct index from 0 -> 3.
			result = result + PS2Resources->EE->EECore->R5900->LO->readWordS(index); 
			PS2Resources->EE->EECore->R5900->LO->writeWordS(index, result);
		}
		else
		{
			auto index = ((i / 2 > 1) ? i - 4 : i - 2); // A2xB2 + C2, A3xB3 + C3, A6xB6 + C6, A7xB7 + C7. Array ternary operator is to select the correct index from 0 -> 3.
			result = result + PS2Resources->EE->EECore->R5900->HI->readWordS(index); 
			PS2Resources->EE->EECore->R5900->HI->writeWordS(index, result);
		}
			
		if (i % 2 == 0)
			destReg->writeWordS(i / 2, result); // Write to Rd for even indexes. A0xB0 + C0, A2xB2 + C2, A4xB4 + C4, A6xB6 + C6.
	}
}

void InterpreterEECore::PMADDUW(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = SignExtend<u64>(Rs[UW] * Rt[UW] + (HI || LO)[i]) (varying indexes - see EE Core Instruction Manual page 218).
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_WORDS_IN_QWORD; i += 2)
	{
		u64 result = source1Reg->readWordU(i) * source2Reg->readWordU(i) + ((static_cast<u64>(PS2Resources->EE->EECore->R5900->LO->readWordU(i)) << 32) || static_cast<u64>(PS2Resources->EE->EECore->R5900->LO->readWordU(i)));

		PS2Resources->EE->EECore->R5900->LO->writeDwordU(i / 2, static_cast<u64>(static_cast<u32>(result & 0xFFFFFFFF)));
		PS2Resources->EE->EECore->R5900->HI->writeDwordU(i / 2, static_cast<u64>(static_cast<u32>(result >> 32)));
		destReg->writeDwordU(i / 2, result);
	}
}

void InterpreterEECore::PMADDW(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = SignExtend<s64>(Rs[SW] * Rt[SW] + (HI || LO)[i]) (varying indexes - see EE Core Instruction Manual page 220).
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_WORDS_IN_QWORD; i += 2)
	{
		s64 result = source1Reg->readWordS(i) * source2Reg->readWordS(i) + ((static_cast<s64>(PS2Resources->EE->EECore->R5900->LO->readWordS(i)) << 32) || static_cast<s64>(PS2Resources->EE->EECore->R5900->LO->readWordS(i)));

		PS2Resources->EE->EECore->R5900->LO->writeDwordS(i / 2, static_cast<s64>(static_cast<s32>(result & 0xFFFFFFFF)));
		PS2Resources->EE->EECore->R5900->HI->writeDwordS(i / 2, static_cast<s64>(static_cast<s32>(result >> 32)));
		destReg->writeDwordS(i / 2, result);
	}
}

void InterpreterEECore::PMSUBH(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = SignExtend<s32>((HI,LO)[i] - Rs[SH] * Rt[SH]) (varying indexes - see EE Core Instruction Manual page 216).
	// (HI,LO)[i] = "C" in comments written below.
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()]; // "A"
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()]; // "B"
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_HWORDS_IN_QWORD; i++)
	{
		s32 result = source1Reg->readHwordS(i) * source2Reg->readHwordS(i);

		if ((i / 2) % 2 == 0)
		{
			auto index = ((i / 2 > 0) ? i - 2 : i); // C0 - A0xB0, C1 - A1xB1, C4 - A4xB4, C5 - A5xB5. Array ternary operator is to select the correct index from 0 -> 3.
			result = PS2Resources->EE->EECore->R5900->LO->readWordS(index) - result;
			PS2Resources->EE->EECore->R5900->LO->writeWordS(index, result);
		}
		else
		{
			auto index = ((i / 2 > 1) ? i - 4 : i - 2); // C2 - A2xB2, C3 - A3xB3, C6 - A6xB6, C7 - A7xB7. Array ternary operator is to select the correct index from 0 -> 3.
			result = PS2Resources->EE->EECore->R5900->HI->readWordS(index) - result;
			PS2Resources->EE->EECore->R5900->HI->writeWordS(index, result);
		}

		if (i % 2 == 0)
			destReg->writeWordS(i / 2, result); // Write to Rd for even indexes. C0 - A0xB0, C2 - A2xB2, C4 - A4xB4, C6 - A6xB6.
	}
}

void InterpreterEECore::PMSUBW(const MIPSInstruction_t & instruction, std::shared_ptr<PS2Resources_t> & PS2Resources)
{
	// (LO, HI, Rd) = SignExtend<s64>((HI || LO)[i] - Rs[SW] * Rt[SW]) (varying indexes - see EE Core Instruction Manual page 220).
	// No Exceptions generated.
	auto& source1Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRs()];
	auto& source2Reg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRt()];
	auto& destReg = PS2Resources->EE->EECore->R5900->GPR[instruction.getRRd()];

	for (auto i = 0; i < Constants::NUMBER_WORDS_IN_QWORD; i += 2)
	{
		s64 result = ((static_cast<s64>(PS2Resources->EE->EECore->R5900->LO->readWordS(i)) << 32) || static_cast<s64>(PS2Resources->EE->EECore->R5900->LO->readWordS(i))) - source1Reg->readWordS(i) * source2Reg->readWordS(i);

		PS2Resources->EE->EECore->R5900->LO->writeDwordS(i / 2, static_cast<s64>(static_cast<s32>(result & 0xFFFFFFFF)));
		PS2Resources->EE->EECore->R5900->HI->writeDwordS(i / 2, static_cast<s64>(static_cast<s32>(result >> 32)));
		destReg->writeDwordS(i / 2, result);
	}
}