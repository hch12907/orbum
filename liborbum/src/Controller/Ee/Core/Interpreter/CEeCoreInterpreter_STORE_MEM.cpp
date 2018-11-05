#include "Controller/Ee/Core/Interpreter/CEeCoreInterpreter.hpp"
#include "Core.hpp"
#include "Resources/RResources.hpp"
#include "Utilities/Utilities.hpp"

void CEeCoreInterpreter::SB(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // MEM[UB] = Rd. Address error or TLB error generated.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_ubyte(BusContext::Ee, *physical_address, reg_source2.read_ubyte(0));
}

void CEeCoreInterpreter::SD(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // MEM[UD] = Rt. Address error or TLB error generated.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_udword(BusContext::Ee, *physical_address, reg_source2.read_udword(0));
}

void CEeCoreInterpreter::SDL(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // TODO: check this, dont think its right. This should work for little-endian architectures (ie: x86), but not sure about big-endian. Luckily most machines are little-endian today, so this may never be a problem.
    // Rd = MEM[UD]. Address error or TLB error generated.
    // Credit to PCSX2.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    uword shift = (virtual_address & 7) << 3;
    uptr dword_address = (virtual_address & 0xFFFFFFF8);

    std::optional<uptr> physical_address = translate_address_data(dword_address, READ);
    if (!physical_address)
        return;

    auto value = r.ee.bus.read_udword(BusContext::Ee, *physical_address);

    physical_address = translate_address_data(dword_address, WRITE); // Need to get phy address again, check for write conditions.
    if (!physical_address)
        return;

    r.ee.bus.write_udword(BusContext::Ee, *physical_address, ((reg_source2.read_udword(0) >> (56 - shift))) | (value & (0xFFFFFFFFFFFFFF00 << shift)));
}

void CEeCoreInterpreter::SDR(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // TODO: check this, dont think its right. This should work for little-endian architectures (ie: x86), but not sure about big-endian. Luckily most machines are little-endian today, so this may never be a problem.
    // Rd = MEM[UD]. Address error or TLB error generated.
    // Credit to PCSX2.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    uword shift = (virtual_address & 7) << 3;
    uptr dword_address = (virtual_address & 0xFFFFFFF8);

    std::optional<uptr> physical_address = translate_address_data(dword_address, READ);
    if (!physical_address)
        return;

    auto value = r.ee.bus.read_udword(BusContext::Ee, *physical_address);

    physical_address = translate_address_data(dword_address, WRITE); // Need to get phy address again, check for write conditions.
    if (!physical_address)
        return;

    r.ee.bus.write_udword(BusContext::Ee, *physical_address, ((reg_source2.read_udword(0) << shift) | (value & (0x00FFFFFFFFFFFFFF >> (56 - shift)))));
}

void CEeCoreInterpreter::SH(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // MEM[UH] = Rd. Address error or TLB error generated.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_uhword(BusContext::Ee, *physical_address, reg_source2.read_uhword(0));
}

void CEeCoreInterpreter::SW(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // MEM[UW] = Rd. Address error or TLB error generated.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_uword(BusContext::Ee, *physical_address, reg_source2.read_uword(0));
}

void CEeCoreInterpreter::SWL(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // TODO: check this, dont think its right. This should work for little-endian architectures (ie: x86), but not sure about big-endian. Luckily most machines are little-endian today, so this may never be a problem.
    // MEM[UW] = Rd. Address error or TLB error generated.
    // Credit to PCSX2.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    uword shift = (virtual_address & 3) << 3;
    uptr word_address = (virtual_address & 0xFFFFFFFC);

    std::optional<uptr> physical_address = translate_address_data(word_address, READ);
    if (!physical_address)
        return;

    auto value = r.ee.bus.read_uword(BusContext::Ee, *physical_address);

    physical_address = translate_address_data(word_address, WRITE); // Need to get phy address again, check for write conditions.
    if (!physical_address)
        return;

    r.ee.bus.write_uword(BusContext::Ee, *physical_address, ((reg_source2.read_uword(0) >> (24 - shift))) | (value & (0xFFFFFF00 << shift)));
}

void CEeCoreInterpreter::SWR(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // TODO: check this, dont think its right. This should work for little-endian architectures (ie: x86), but not sure about big-endian. Luckily most machines are little-endian today, so this may never be a problem.
    // MEM[UW] = Rd. Address error or TLB error generated.
    // Credit to PCSX2.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    uword shift = (virtual_address & 3) << 3;
    uptr word_address = (virtual_address & 0xFFFFFFFC);

    std::optional<uptr> physical_address = translate_address_data(word_address, READ);
    if (!physical_address)
        return;

    auto value = r.ee.bus.read_uword(BusContext::Ee, *physical_address);

    physical_address = translate_address_data(word_address, WRITE); // Need to get phy address again, check for write conditions.
    if (!physical_address)
        return;

    r.ee.bus.write_uword(BusContext::Ee, *physical_address, ((reg_source2.read_uword(0) << shift) | (value & (0x00FFFFFF >> (24 - shift)))));
}

void CEeCoreInterpreter::SQ(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // MEM[UQ] = Rd. Address error or TLB error generated.
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.core.r5900.gpr[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_uqword(BusContext::Ee, *physical_address, reg_source2.read_uqword());
}

void CEeCoreInterpreter::SWC1(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // MEM[UW] = Ft. Address error or TLB error generated.
    if (!handle_cop1_usable())
        return;

    const auto& reg_source2 = r.ee.core.fpu.fpr[inst.rt()];   // Ft
    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const shword imm = inst.s_imm();

    uptr virtual_address = (reg_source1.read_uword(0) + imm);
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_uword(BusContext::Ee, *physical_address, reg_source2.read_uword());
}

void CEeCoreInterpreter::SQC2(const EeCoreInstruction inst)
{
    auto& r = core->get_resources();

    // VU0.vf = GPR. Coprocessor unusable exception.
    if (!handle_cop2_usable())
        return;

    const auto& reg_source1 = r.ee.core.r5900.gpr[inst.rs()]; // "Base"
    const auto& reg_source2 = r.ee.vpu.vu.unit_0.vf[inst.rt()];
    const shword imm = inst.s_imm();

    uptr virtual_address = reg_source1.read_uword(0) + imm;
    std::optional<uptr> physical_address = translate_address_data(virtual_address, WRITE);
    if (!physical_address)
        return;

    r.ee.bus.write_uqword(BusContext::Ee, *physical_address, reg_source2.read_uqword());
}
