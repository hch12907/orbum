#include "stdafx.h"

#include "PS2Resources/Common/Types/SBUSRegisters_t.h"

SBUSRegister_MSCOM_t::SBUSRegister_MSCOM_t() :
	Register32_t("SBUS: MSCOM")
{
}

void SBUSRegister_MSCOM_t::writeWord(const Context_t& context, u32 value)
{
	if (context != Context_t::IOP)
		Register32_t::writeWord(context, value);
}

SBUSRegister_MSFLG_t::SBUSRegister_MSFLG_t() :
	Register32_t("SBUS: MSFLG")
{
}

void SBUSRegister_MSFLG_t::writeWord(const Context_t& context, u32 value)
{
	if (context == Context_t::EE)
		Register32_t::writeWord(context, readWord(context) | value);
	else if (context == Context_t::IOP)
		Register32_t::writeWord(context, readWord(context) & (~value));
	else
		Register32_t::writeWord(context, value);
}

SBUSRegister_SMFLG_t::SBUSRegister_SMFLG_t() :
	Register32_t("SBUS: SMFLG")
{
}

void SBUSRegister_SMFLG_t::writeWord(const Context_t& context, u32 value)
{
	if (context == Context_t::EE)
		Register32_t::writeWord(context, readWord(context) & (~value));
	else if (context == Context_t::IOP)
		Register32_t::writeWord(context, readWord(context) | value);
	else
		Register32_t::writeWord(context, value);
}

SBUSRegister_F240_t::SBUSRegister_F240_t() :
	Register32_t("SBUS: F240")
{
}

u16 SBUSRegister_F240_t::readHword(const Context_t& context, u32 arrayIndex)
{
	if (context == Context_t::IOP && arrayIndex == 0)
		return (Register32_t::readHword(context, arrayIndex) | 0x2);
	else
		return Register32_t::readHword(context, arrayIndex);
}

u32 SBUSRegister_F240_t::readWord(const Context_t& context)
{
	if (context == Context_t::EE)
		return (Register32_t::readWord(context) | 0xF0000102);
	else if (context == Context_t::IOP)
		return (Register32_t::readWord(context) | 0xF0000002);
	else
		return Register32_t::readWord(context);
}

void SBUSRegister_F240_t::writeHword(const Context_t& context, u32 arrayIndex, u16 value)
{
	if (context == Context_t::IOP && arrayIndex == 0)
	{
		u16 temp = value & 0xF0;
		if (value & 0x20 || value & 0x80)
		{
			Register32_t::writeHword(context, arrayIndex, Register32_t::readHword(context, arrayIndex) & (~0xF000));
			Register32_t::writeHword(context, arrayIndex, Register32_t::readHword(context, arrayIndex) | 0x2000);
		}

		if (Register32_t::readHword(context, arrayIndex) & temp)
			Register32_t::writeHword(context, arrayIndex, Register32_t::readHword(context, arrayIndex) & (~temp));
		else
			Register32_t::writeHword(context, arrayIndex, Register32_t::readHword(context, arrayIndex) | temp);
	}
	else
	{
		Register32_t::writeHword(context, arrayIndex, value);
	}
}

void SBUSRegister_F240_t::writeWord(const Context_t& context, u32 value)
{
	if (context == Context_t::EE)
	{
		u32 temp = Register32_t::readWord(context) & 0xFFFFFEFF;
		temp |= value & 0x100;
		Register32_t::writeWord(context, temp);
	}
	else if (context == Context_t::IOP)
	{
		u32 temp = value & 0xF0;
		if (value & 0x20 || value & 0x80)
		{
			Register32_t::writeWord(context, Register32_t::readWord(context) & (~0xF000));
			Register32_t::writeWord(context, Register32_t::readWord(context) | 0x2000);
		}

		if (Register32_t::readWord(context) & temp)
			Register32_t::writeWord(context, Register32_t::readWord(context) & (~temp));
		else
			Register32_t::writeWord(context, Register32_t::readWord(context) | temp);
	}
	else
	{
		Register32_t::writeWord(context, value);
	}
}

SBUSRegister_F300_t::SBUSRegister_F300_t() :
	Register32_t("SBUS: F300")
{
}

u32 SBUSRegister_F300_t::readWord(const Context_t& context)
{
	throw std::runtime_error("SBUS_F300 not implemented.");
}

void SBUSRegister_F300_t::writeWord(const Context_t& context, u32 value)
{
	throw std::runtime_error("SBUS_F300 not implemented.");
}