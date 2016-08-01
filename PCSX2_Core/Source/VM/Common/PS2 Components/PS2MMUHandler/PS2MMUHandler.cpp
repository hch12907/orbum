#include "stdafx.h"

#include <cmath>

#include "Common/Global/Globals.h"

#include "VM/Common/PS2 Components/PS2MMUHandler/PS2MMUHandler.h"
#include "VM/VMMain.h"
#include "Common/PS2 Types/PS2Exception/PS2Exception_t.h"


PS2MMUHandler::PS2MMUHandler(const VMMain *const vmMain) : VMPS2Component(vmMain)
{
}

u32 PS2MMUHandler::getPS2PhysicalAddress_Stage1(u32 PS2VirtualAddress, AccessType accessType) const
{
	// This process follows the information and diagram given on page 121 & 122 of the EE Core Users Manual. 
	// I am unsure if this is exactly what happens, as the information is a bit vague on how to obtain the page mask and ASID, 
	//  but I'm confident this is what it meant (I also dont see another way to do it).
	// Note: In the kernel context mode, for an VA in kseg0 and kseg1 a physical address is immediately returned, as they are both unmapped - no translation occurs.

	// BIG NOTE: where the lower bound of memory segments is 0x00000000, it has been omitted from the test statement (as it is implied through the use of u32).

	// Provide convencience access to these variables (KSU, ERL, EXL).
	auto KSU = getVM()->getResources()->EE.EECore.COP0.BitfieldRegisters.Status.getFieldValue(RegisterStatus_t::Fields::KSU);
	auto ERL = getVM()->getResources()->EE.EECore.COP0.BitfieldRegisters.Status.getFieldValue(RegisterStatus_t::Fields::ERL);
	auto EXL = getVM()->getResources()->EE.EECore.COP0.BitfieldRegisters.Status.getFieldValue(RegisterStatus_t::Fields::EXL);

	// Step 1 is to determine which CPU context we are in (user, supervisor or kernel).
	// User mode when KSU = 2, ERL = 0, EXL = 0 in the status register.
	if (KSU == 2 && ERL == 0 && EXL == 0)
	{
		// Operating in user mode.
		// First we check if the VA is within the context bounds.
		if (PS2VirtualAddress > PS2Constants::EE::EECore::MMU::VADDRESS_USER_UPPER_BOUND)
		{
			// Throw address error if not within bounds.
			if (accessType == READ) PS2Exception_t a(PS2Exception_t::ExceptionType::EX_ADDRESS_ERROR_INSTRUCTION_FETCH_LOAD);
			else if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_ADDRESS_ERROR_STORE);
			else throw std::runtime_error("PS2MMUHandler: could not throw internal PS2Exception_t error (type = address error).");
		}

		return getPS2PhysicalAddress_Stage2(PS2VirtualAddress, accessType);
	}
	else if (KSU == 1 && ERL == 0 && EXL == 0)
	{
		// Operating in supervisor mode.
		// First we check if the VA is within the context bounds.
		if ((PS2VirtualAddress > PS2Constants::EE::EECore::MMU::VADDRESS_SUPERVISOR_UPPER_BOUND_1 && PS2VirtualAddress < PS2Constants::EE::EECore::MMU::VADDRESS_SUPERVISOR_LOWER_BOUND_2) ||
			(PS2VirtualAddress > PS2Constants::EE::EECore::MMU::VADDRESS_SUPERVISOR_UPPER_BOUND_2))
		{
			// Throw address error if not within bounds.
			if (accessType == READ) PS2Exception_t a(PS2Exception_t::ExceptionType::EX_ADDRESS_ERROR_INSTRUCTION_FETCH_LOAD);
			else if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_ADDRESS_ERROR_STORE);
			else throw std::runtime_error("PS2MMUHandler: could not throw internal PS2Exception_t error (type = address error).");
		}

		return getPS2PhysicalAddress_Stage2(PS2VirtualAddress, accessType);
	} 
	else if ((KSU == 0 && EXL == 1) || ERL == 1)
	{
		// Operating in kernel mode.
		// We do not need to check if the VA is valid - it is always valid over the full 4GB (U32) address space. However, kseg0 and kseg1 are not mapped, 
		//  and instead directly translate to physical addresses from 0x00000000 -> 0x1FFFFFFF (we need to emulate this). We also need to test for if
		//  Status.ERL == 1, in which case kuseg is unmapped. See EE Core Users Manual page 113 (especially notes).

		// Test for kseg0
		if (PS2VirtualAddress >= PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_LOWER_BOUND_2 && PS2VirtualAddress <= PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_UPPER_BOUND_2)
		{
			// We are in kseg0, so to get the physical address we just minus the kseg0 base address of 0x80000000.
			return (PS2VirtualAddress - PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_LOWER_BOUND_2);
		}

		// Test for kseg1
		if (PS2VirtualAddress >= PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_LOWER_BOUND_3 && PS2VirtualAddress <= PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_UPPER_BOUND_3)
		{
			// We are in kseg1, so to get the physical address we just minus the kseg1 base address of 0xA0000000.
			return (PS2VirtualAddress - PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_LOWER_BOUND_3);
		}
		
		// Test for Status.ERL = 1 (indicating kuseg is unmapped). Note that the VA still has to be within the segment bounds for this to work.
		if (ERL == 1) {
			if (PS2VirtualAddress <= PS2Constants::EE::EECore::MMU::VADDRESS_KERNEL_UPPER_BOUND_1)
			{
				// We are in kuseg unmapped region, so just return the VA.
				return PS2VirtualAddress;
			}
		}

		// Else in a mapped region - do normal translation.
		return getPS2PhysicalAddress_Stage2(PS2VirtualAddress, accessType);
	}
	else
	{
		// Throw runtime error as this should never happen.
		throw std::runtime_error("PS2MMUHandler: could not determine context. Needs debugging!");
	}
}

u32 PS2MMUHandler::getPS2PhysicalAddress_Stage2(const u32 & PS2VirtualAddress, const AccessType & accessType) const
{
	// Stage 2 is to search through the TLB to see if there is a VPN match. 
	// Check if its in the TLB and get the information.
	s32 index = getTLBIndex(PS2VirtualAddress);
	if (index == -1)
	{
		// A match was not found, throw a TLB miss PS2 exception.
		// Throw address error if not within bounds.
		if (accessType == READ) PS2Exception_t a(PS2Exception_t::ExceptionType::EX_TLB_REFILL_INSTRUCTION_FETCH_LOAD);
		else if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_TLB_REFILL_STORE);
		else throw std::runtime_error("PS2MMUHandler: could not throw internal PS2Exception_t error (type = tlb refill).");
	}
	auto tlbEntry = mTLBEntries[index];

	// Check the global bit, and check ASID if needed (against the ASID value in the EntryHi COP0 register).
	if (tlbEntry.mG == 0)
	{
		// Not a global page map, need to make sure ASID's are the same.
		if (getVM()->getResources()->EE.EECore.COP0.BitfieldRegisters.EntryHi.getFieldValue(RegisterEntryHi_t::Fields::ASID) != tlbEntry.mASID)
		{
			// Generate TLB refill exception.
			if (accessType == READ) PS2Exception_t a(PS2Exception_t::ExceptionType::EX_TLB_REFILL_INSTRUCTION_FETCH_LOAD);
			else if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_TLB_REFILL_STORE);
			else throw std::runtime_error("PS2MMUHandler: could not throw internal PS2Exception_t error (type = tlb refill).");
		}
	}

	// Move on to stage 3.
	return getPS2PhysicalAddress_Stage3(PS2VirtualAddress, accessType, tlbEntry);
}

u32 PS2MMUHandler::getPS2PhysicalAddress_Stage3(u32 PS2VirtualAddress, AccessType accessType, const TLBEntryInformation & tlbEntry) const
{
	// Stage 3: Assess if the page is valid and it is marked dirty.
	// Need to check now before continuing if the VPN is for a even or odd page. This is done by checking the LSB of the VPN from the original address accessed.
	// Neat trick: +1 to the TLB mask to get the mask for the LSB of the VPN.
	if (PS2VirtualAddress & (tlbEntry.mMask + 1))
	{
		// Odd
		// Check if the entry is valid (V bit)
		if (!tlbEntry.mVOdd)
		{
			// Raise TLB invalid exception
			if (accessType == READ) PS2Exception_t a(PS2Exception_t::ExceptionType::EX_TLB_INVALID_INSTRUCTION_FETCH_LOAD);
			else if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_TLB_INVALID_STORE);
			else throw std::runtime_error("PS2MMUHandler: could not throw internal PS2Exception_t error (type = tlb invalid).");
		}	
		
		// Check if entry is allowed writes (dirty flag)
		if (!tlbEntry.mDOdd)
		{
			// Raise TLB modified exception if writing occurs.
			if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_TLB_MODIFIED);
		}

		// Move on to stage 4 (odd)
		return getPS2PhysicalAddress_Stage4Odd(PS2VirtualAddress, accessType, tlbEntry);
	}
	else
	{
		// Even
		// Check if the entry is valid (V bit)
		if (!tlbEntry.mVEven)
		{
			// Raise TLB invalid exception
			if (accessType == READ) PS2Exception_t a(PS2Exception_t::ExceptionType::EX_TLB_INVALID_INSTRUCTION_FETCH_LOAD);
			else if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_TLB_INVALID_STORE);
			else throw std::runtime_error("PS2MMUHandler: could not throw internal PS2Exception_t error (type = tlb invalid).");
		}

		// Check if entry is allowed writes (dirty flag)
		if (!tlbEntry.mDEven)
		{
			// Raise TLB modified exception if writing occurs.
			if (accessType == WRITE) throw PS2Exception_t(PS2Exception_t::ExceptionType::EX_TLB_MODIFIED);
		}

		// Move on to stage 4 (odd)
		return getPS2PhysicalAddress_Stage4Even(PS2VirtualAddress, accessType, tlbEntry);
	}
}

u32 PS2MMUHandler::getPS2PhysicalAddress_Stage4Odd(u32 PS2VirtualAddress, AccessType accessType, const TLBEntryInformation& tlbEntry) const
{
	// Check if accessing scratchpad
	if (tlbEntry.mS)
	{
		// TODO: implement accessing scratchpad. Idea on how to do this: map the reserved region 0x14000000 -> 0x1FBFFFFF to point to the SPRAM, using the VM MMU as this region will be unmapped anyway. The reserved region is 192 MB, plenty of space.
		return 0;
	}

	// Or cache access?
	if (tlbEntry.mCOdd > 0)
	{
		// TODO: implement accessing cache. Idea on how to do this: map the reserved region 0x14000000 -> 0x1FBFFFFF to point to the cache using the VM MMU as this region will be unmapped anyway. The reserved region is 192 MB, plenty of space.
		return 0;
	}

	// Else we are accessing main memory.
	// Combine PFN with offset using the TLB entry mask.
	u32 PFNBitPos = static_cast<u32>(log2(tlbEntry.mMask + 1));
	return ((tlbEntry.mPFNOdd << PFNBitPos) || (PS2VirtualAddress & tlbEntry.mMask));
}

u32 PS2MMUHandler::getPS2PhysicalAddress_Stage4Even(u32 PS2VirtualAddress, AccessType accessType, const TLBEntryInformation& tlbEntry) const
{
	// Check if accessing scratchpad
	if (tlbEntry.mS)
	{
		// TODO: implement accessing scratchpad. Idea on how to do this: map the reserved region 0x14000000 -> 0x1FBFFFFF to point to the SPRAM, using the VM MMU as this region will be unmapped anyway. The reserved region is 192 MB, plenty of space.
		return 0;
	}

	// Or cache access?
	if (tlbEntry.mCEven > 0)
	{
		// TODO: implement accessing cache. Idea on how to do this: map the reserved region 0x14000000 -> 0x1FBFFFFF to point to the cache using the VM MMU as this region will be unmapped anyway. The reserved region is 192 MB, plenty of space.
		return 0;
	}

	// Else we are accessing main memory.
	// Combine PFN with offset using the TLB entry mask.
	u32 PFNBitPos = static_cast<u32>(log2(tlbEntry.mMask + 1));
	return ((tlbEntry.mPFNEven << PFNBitPos) || (PS2VirtualAddress & tlbEntry.mMask));
}

s32 PS2MMUHandler::getTLBIndex(u32 PS2VirtualAddress) const
{
	// Since the VPN can be a number of sizes, we start with the largest page size of 16MB or the most significant 8 bits (which means smallest VPN number) and search for that.
	// If a match is found, then we get the proper page size from the TLB entry and check to see if the VA is within the page. If not we continue searching.
	// Note that the TLB entries' VPN are stored as VPN / 2 (aka VPN2), so we need to search by using the same. This is done as each VPN represents 2 physical pages (named 'even' and 'odd').
	//  Therefore the LSB of the VPN is ommitted and only 19 bits are used at most. I am not 100% sure on the details on this, however. If the above is true, then we need to search using the
	//  last 7 bits.
	u32 searchVPN2 = (PS2VirtualAddress >> 25); // End up with 7 bits. Normally this would be ((value >> 24) / 2) but dividing by 2 is exactly the same as shifting it right by 1 (ie: >> 25).
	for (auto i = 0; i < PS2Constants::EE::EECore::MMU::SIZE_TLB_ENTRIES; i++)
	{
		// Even though in a real tlb entry the VPN2 field uses bits 77-95 (length 19), we have stored it in a separate u32 type (ie: from bits 0-18), so we need to mask out bits 12-18 (length 7).
		auto tlbEntry = mTLBEntries[i];
		u32 tlbMaskedVPN2 = tlbEntry.mVPN2 & MASK_VPN2_FIELD;
		if (searchVPN2 == tlbMaskedVPN2)
		{
			// A potential match was found. Need to now check if the VPN is actually correct by using the TLB entry mask.
			u32 realVPN2 = (PS2VirtualAddress & ~tlbEntry.mMask) / 2;
			if (realVPN2 == tlbEntry.mVPN2)
			{
				// A match was found. I will assume that there will never be 2 entries with the same VPN.
				return i;
			}
		}
	}

	// No match was found, return -1 for the caller to deal with.
	return -1;
}
