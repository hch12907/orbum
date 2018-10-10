#pragma once

#include <cereal/cereal.hpp>

#include "Common/Constants.hpp"
#include "Common/Types/Mips/MipsCoprocessor.hpp"
#include "Common/Types/Register/SizedWordRegister.hpp"
#include "Resources/Ee/Core/EeCoreCop0.hpp"
#include "Resources/Ee/Core/EeCoreFpuRegisters.hpp"

/// FPU refers to the FPU (floating-point unit) coprocessor, attached as COP1.
/// See EE Core Users Manual, chapter 8.
class EeCoreFpu : public MipsCoprocessor
{
public:
    EeCoreFpu();

    /// Checks if the FPU coprocessor (FPU) is usable, by looking at the COP0 CU[1] bit.
    /// CU[1] > 0 means FPU is usable.
    bool is_usable() override;

    /// 32 general purpose registers, called FPR's.
    /// See EE Core Users Manual, page 157.
    SizedWordRegister fpr[Constants::EE::EECore::FPU::NUMBER_REGISTERS];

    /// Accumulator register, used for multiply-accumulate type instructions.
    /// See EE Core Users Manual, page 157.
    SizedWordRegister acc;

    /// FPU defines 2 control registers, which are implemented as bitfield registers.
    /// See EE Core Users Manual, page 158.
    EeCoreFpuRegister_Irr irr;
    EeCoreFpuRegister_Csr csr;

    /// Array of control registers.
    /// Only FCR[0] and [31] are defined.
    SizedWordRegister* fcr[Constants::EE::EECore::FPU::NUMBER_REGISTERS];

    /// Pointer to the EE Core COP0 coprocessor, needed for the Status register.
    EeCoreCop0* cop0;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(fpr),
            CEREAL_NVP(acc),
            CEREAL_NVP(irr),
            CEREAL_NVP(csr));
    }
};
