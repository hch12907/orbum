#pragma once


#include "VM/Component Interfaces/VMInterpreterComponent.h"
#include "Common/PS2 Types/MIPSInstruction/MIPSInstruction_t.h"

class PS2Resources_t;
class VMMain;
class InterpreterR5900 : public VMInterpreterComponent
{
public:
	InterpreterR5900(const VMMain *const vmMain, const Interpreter *const interpreter);
	~InterpreterR5900();

	/*
	This is the "main loop" function called by the base interpreter component.
	*/
	void runInterpreterComponent() override;

private:

	// Component state functions

	/*
	The is used as a temporary holder for the current instruction, while the operation to perform is being determined.
	*/
	MIPSInstruction_t mInstruction;

	// Static R5900 Instruction functions. The instructions are organised according to the EE Overview Manual starting from page 26 (which also means separate cpp files per category).

	/*
	Instruction Table. This table provides pointers to instruction implementations, which is accessed by the implementation index. See R5900InstructionUtil for more details.
	*/
	static void(*const R5900_INSTRUCTION_TABLE[Constants::NUMBER_R5900_INSTRUCTIONS])(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);

	/*
	Unknown instruction function - does nothing when executed. Used for any instructions with implementation index 0 (ie: reserved, unknown or otherwise).
	If the PCSX2_DEBUG macro is enabled, can be used to debug an unknown opcode by logging a message.
	Will increase PC by 4 regardless.
	*/
	static void INSTRUCTION_UNKNOWN(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);

	/*
	Integer Add/Sub Instructions. See InterpreterR5900_INTEGER_ADD_SUB.cpp for implementations (31 instructions total).
	*/
	static void ADD(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void ADDI(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void ADDIU(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void ADDU(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void DADD(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void DADDI(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void DADDIU(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void DADDU(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void DSUB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void DSUBU(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void SUB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void SUBU(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDSB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDSH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDSW(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDUB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDUH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDUW(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADDW(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PADSBH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBSB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBSH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBSW(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBUB(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBUH(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBUW(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
	static void PSUBW(const MIPSInstruction_t & instruction, PS2Resources_t & PS2Resources);
};

