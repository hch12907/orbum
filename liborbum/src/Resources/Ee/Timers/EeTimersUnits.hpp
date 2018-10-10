#pragma once

#include <cereal/cereal.hpp>

#include "Common/Types/Register/SizedWordRegister.hpp"
#include "Resources/Ee/Timers/EeTimersUnitRegisters.hpp"

/// EE Timer unit abstraction type.
class EeTimersUnit
{
public:
    EeTimersUnit();

    int* unit_id;

    EeTimersUnitRegister_Count* count;
    EeTimersUnitRegister_Mode* mode;
    SizedWordRegister* compare;
    SizedWordRegister* hold;
};

/// A base type representing an EE Timers Timer.
/// See EE Users Manual page 35.
/// The MODE, COUNT and COMP registers are always defined for each timer.
class EeTimersUnit_Base
{
public:
    EeTimersUnit_Base(const int unit_id);

    int unit_id;

    EeTimersUnitRegister_Count count;
    EeTimersUnitRegister_Mode mode;
    SizedWordRegister compare;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(count),
            CEREAL_NVP(mode),
            CEREAL_NVP(compare));
    }
};

/// EE Timer with HOLD register, for timers 0 and 1.
class EeTimersUnit_Hold : public EeTimersUnit_Base
{
public:
    EeTimersUnit_Hold(const int unit_id);

    SizedWordRegister hold;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::base_class<EeTimersUnit_Base>(this),
            CEREAL_NVP(hold));
    }
};