#include "stdafx.h"

#include "Common/Global/Globals.h"
#include "Common/Types/Registers/Register128_t.h"

Register128_t::Register128_t() : 
	UD{ 0 }
{
}

u8 Register128_t::readByteU(u32 arrayIndex)
{
	return UB[arrayIndex];
}

void Register128_t::writeByteU(u32 arrayIndex, u8 value)
{
	UB[arrayIndex] = value;
}

s8 Register128_t::readByteS(u32 arrayIndex)
{
	return SB[arrayIndex];
}

void Register128_t::writeByteS(u32 arrayIndex, s8 value)
{
	SB[arrayIndex] = value;
}

u16 Register128_t::readHwordU(u32 arrayIndex)
{
	return UH[arrayIndex];
}

void Register128_t::writeHwordU(u32 arrayIndex, u16 value)
{
	UH[arrayIndex] = value;
}

s16 Register128_t::readHwordS(u32 arrayIndex)
{
	return SH[arrayIndex];
}

void Register128_t::writeHwordS(u32 arrayIndex, s16 value)
{
	SH[arrayIndex] = value;
}

u32 Register128_t::readWordU(u32 arrayIndex)
{
	return UW[arrayIndex];
}

void Register128_t::writeWordU(u32 arrayIndex, u32 value)
{
	UW[arrayIndex] = value;
}

s32 Register128_t::readWordS(u32 arrayIndex)
{
	return SW[arrayIndex];
}

void Register128_t::writeWordS(u32 arrayIndex, s32 value)
{
	SW[arrayIndex] = value;
}

u64 Register128_t::readDwordU(u32 arrayIndex)
{
	return UD[arrayIndex];
}

void Register128_t::writeDwordU(u32 arrayIndex, u64 value)
{
	UD[arrayIndex] = value;
}

s64 Register128_t::readDwordS(u32 arrayIndex)
{
	return SD[arrayIndex];
}

void Register128_t::writeDwordS(u32 arrayIndex, s64 value)
{
	SD[arrayIndex] = value;
}