#pragma once

#include "Common/Global/Globals.h"

/*
FPRegister128_t is a register type used for VU floating point operations. It is made up of 4 x 32-bit single precision floats, and not intented to be a 128-bit 'float' type.
All FPR's are 32-bit (single precision) according to the documentation, and the floating point array is defined by the 'F' union field below.

See the VU Users Manual page 16 onwards.

Some additional functions are exposed for manipulating the floating point fields (sign, exponent and fraction), which is done by manipulating the u32 UW in the union defined.

The FPU and the floating point values in general are NOT FULLY IEEE-754 compliant (mostly, but not fully).
In particular, the FPU does not support (in comparison to IEEE-754):
 - Denormalised numbers.
 - Plus and minus infinity.
 - 'NaN' (not a number) representation.
*/
class FPRegister128_t
{
public:
	// Initialise union with 0 value.
	explicit FPRegister128_t();

	virtual ~FPRegister128_t()
	{
	}

	union
	{
		u32 UW[Constants::NUMBER_WORDS_IN_QWORD];
		s32 SW[Constants::NUMBER_WORDS_IN_QWORD];
		f32 F[Constants::NUMBER_WORDS_IN_QWORD];
	};

	/*
	Functions to access the register value - you should use these functions instead of accessing them directly.
	NOTE: IT IS UP TO THE USER TO MAKE SURE THE FLOAT VALUE WRITTEN IS COMPATIBLE WITH THE PS2! Use the FPUUtil static class functions to help with conversion.
	*/
	virtual u32 readWordU(u32 arrayIndex);
	virtual void writeWordU(u32 arrayIndex, u32 value);
	virtual s32 readWordS(u32 arrayIndex);
	virtual void writeWordS(u32 arrayIndex, s32 value);
	virtual f32 readFloat(u32 arrayIndex);
	virtual void writeFloat(u32 arrayIndex, f32 value);
};
