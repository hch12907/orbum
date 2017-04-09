#include "stdafx.h"

#include "Common/Global/Globals.h"
#include "Common/Types/Register/Register16_t.h"

Register16_t::Register16_t(const char* mnemonic, bool debugReads, bool debugWrites) :
	DebugBaseObject_t(mnemonic, debugReads, debugWrites),
	UH(0)
{
}

u8 Register16_t::readByte(const System_t context, size_t arrayIndex)
{
#if defined(BUILD_DEBUG)
	if (mDebugReads)
	{
#if DEBUG_LOG_VALUE_AS_HEX
		log(Debug, "%s: %s Read u8[%d], Value = 0x%X.", getSystemStr(context), mMnemonic.c_str(), arrayIndex, UB[arrayIndex]);
#else
		log(Debug, "%s: %s Read u8[%d], Value = %d.", getSystemStr(context), mMnemonic.c_str(), arrayIndex, UB[arrayIndex]);
#endif
	}
#endif

	return UB[arrayIndex];
}

void Register16_t::writeByte(const System_t context, size_t arrayIndex, u8 value)
{
	UB[arrayIndex] = value;

#if defined(BUILD_DEBUG)
	if (mDebugWrites)
	{
#if DEBUG_LOG_VALUE_AS_HEX
		log(Debug, "%s: %s Write u8[%d], Value = 0x%X.", getSystemStr(context), mMnemonic.c_str(), arrayIndex, UB[arrayIndex]);
#else
		log(Debug, "%s: %s Write u8[%d], Value = %d.", getSystemStr(context), mMnemonic.c_str(), arrayIndex, UB[arrayIndex]);
#endif
	}
#endif
}

u16 Register16_t::readHword(const System_t context)
{
#if defined(BUILD_DEBUG)
	if (mDebugReads)
	{
#if DEBUG_LOG_VALUE_AS_HEX
		log(Debug, "%s: %s Read u16, Value = 0x%X.", getSystemStr(context), mMnemonic.c_str(), UH);
#else
		log(Debug, "%s: %s Read u16, Value = %d.", getSystemStr(context), mMnemonic.c_str(), UH);
#endif
	}
#endif

	return UH;
}

void Register16_t::writeHword(const System_t context, u16 value)
{
	UH = value;

#if defined(BUILD_DEBUG)
	if (mDebugWrites)
	{
#if DEBUG_LOG_VALUE_AS_HEX
		log(Debug, "%s: %s Write u16, Value = 0x%X.", getSystemStr(context), mMnemonic.c_str(), UH);
#else
		log(Debug, "%s: %s Write u16, Value = %d.", getSystemStr(context), mMnemonic.c_str(), UH);
#endif
	}
#endif
}

void Register16_t::initalise()
{
	UH = 0;
}