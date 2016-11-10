#pragma once

#include <memory>

/*
TODO: finish writing documentation, and to create copy constructors for each of the elements (looking into saving/restoring the state).
PS2Resources_t delares all of the stateful resources attached to the PS2, such as the EE memory, GS memory and more.
All of the reads and writes to the PS2 system state will come to here.

The structure of PS2Resources_t is defined as a hierarchy of 'components', with the components defined in the Types subfolder within each 'category'.
Categories are loosely based off the chapters in the EE Users Manual.
There is no hard definition of what is a component and what is a category, although rule of thumb you shouldn't make a component too big to understand.
User is to make a judgement, just be sure to make clear your intentions.
As an example, the EE & DMAC is defined as a category, and inside is the DMAC channel components and common registers.

You should only ever need to create an object of the PS2Resources_t type from which you can access everything,
which is ordered by components in the documentation groups, such as PS2Resources->PS2State->EE->EECore->R5900->GPR[1]->readWordU().

Subobjects of this resources class should be extended from the PS2ResourcesSubobject interface if required - this is so they may inspect other resources
on function calls. An example of this is for some R5900 registers (ie: LinkRegister), where they can set other registers automatically.

Note: In order to reduce compile times, try to define the subobjects in the classes initalisation list, and use forward delarations where possible 
This applies to every object defined in this hierarchy. Yes, it looks ugly. 
TODO: find a better solution? Don't think there is one besides sacraficing more compile time.

*/

class EE_t;
class GS_t;
class IOP_t;
class Clock_t;

class PS2Resources_t
{
public:
	explicit PS2Resources_t();

	/*
	The Clock class, used to control timing accross components.
	*/
	std::shared_ptr<Clock_t> Clock;

	/*
	The GS structure.
	*/
	std::shared_ptr<GS_t> GS;

	/* 
	The EE structure.
	*/
	std::shared_ptr<EE_t> EE;

	/*
	The IOP structure.
	*/
	std::shared_ptr<IOP_t> IOP;


private:
	/*
	Initalise the EE and IOP physical memory space. 
	Has to be done after the sub classes above have been initalised otherwise null pointer errors can occur.
	*/
	void mapPhysicalMemory_EE() const;
	void mapPhysicalMemory_IOP() const;
	// void mapPhysicalMemory_VU0() const;
};