#pragma once

#include "Common/Interfaces/VMExecutionCoreComponent.h"

class INTCHandler : public VMExecutionCoreComponent
{
public:
	explicit INTCHandler(const VMMain* const vmMain);

	/*
	Checks the I_STAT register for any pending interrupts. If there are, AND's it with the I_MASK register and checks for any
	interrupts to send to the EE Core to process.

	Note that the maximum time resolution that this can run at is limited to BUSCLK - the reason for this being that all interrupt sources run
	 at BUSCLK or less. Therefore it should be included with the BUSCLK events in the main emulation loop.
	*/
	void executionStep() override;
};
