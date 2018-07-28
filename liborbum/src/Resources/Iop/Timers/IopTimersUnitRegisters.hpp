#pragma once

#include "Common/Types/Register/SizedWordRegister.hpp"
#include "Common/Types/ScopeLock.hpp"
#include "Controller/ControllerEvent.hpp"

using ControllerEventType = ControllerEvent::Type;

/// Timer count register.
/// Can either operate in 16-bit mode (timers 0 -> 2) or 32-bit mode (timers 3 -> 5).
/// This register is only accessed by the Timers controller, it doesn't need any sync support.
class IopTimersUnitRegister_Count : public SizedWordRegister
{
public:
    IopTimersUnitRegister_Count(const bool b32_mode);

    /// Increments the timer by the amount specified (controlling prescalling when required).
    /// Updates the overflow status. If it overflows, the flag is set.
    void increment(const uword value);

    /// Returns if the register has overflowed or not, and resets the flag.
    bool is_overflowed_and_reset();

    /// Reset the count with the specified prescale.
    void reset_prescale(const int prescale_target);

private:
    /// 32 or 16-bit increment mode selector.
    bool is_using_32b_mode;

    /// 16-bit increment.
    void increment_16(const uhword value);

    /// 32-bit increment.
    void increment_32(const uword value);

    /// Internal overflow flag. Use/see is_overflowed_and_reset().
    bool is_overflowed;

    /// Prescale functionality, see increment().
    /// (ie: needs x amount before 1 is added to the count).
    int prescale_target;
    int prescale_count;
};

/// The Timer Mode register type.
/// The PS2SDK has some useful information: https://github.com/ps2dev/ps2sdk/blob/master/iop/kernel/include/timrman.h (carefull - the bit(s) column is 1-indexed, not 0-indexed!).
/// See also the No$PSX docs: http://problemkaputt.de/psx-spx.htm#timers.
///
/// TODO: might not need to be scope locked... once IOP writes mode, its not touched again
/// until interrupt is hit - at which point the controller has already written data way before.
/// 99% of time controller only reads, never writes.
class IopTimersUnitRegister_Mode : public SizedWordRegister, public ScopeLock
{
public:
    static constexpr Bitfield SYNC_ENABLE = Bitfield(0, 1);
    static constexpr Bitfield SYNC_MODE = Bitfield(1, 2);
    static constexpr Bitfield RESET_MODE = Bitfield(3, 1);
    static constexpr Bitfield IRQ_ON_TARGET = Bitfield(4, 1);
    static constexpr Bitfield IRQ_ON_OF = Bitfield(5, 1);
    static constexpr Bitfield IRQ_REPEAT = Bitfield(6, 1);
    static constexpr Bitfield IRQ_TOGGLE = Bitfield(7, 1);
    static constexpr Bitfield EVENT_SRC = Bitfield(8, 1);
    static constexpr Bitfield PRESCALE0 = Bitfield(9, 1);
    static constexpr Bitfield IRQ_REQUEST = Bitfield(10, 1);
    static constexpr Bitfield REACH_TARGET = Bitfield(11, 1);
    static constexpr Bitfield REACH_OF = Bitfield(12, 1);
    static constexpr Bitfield PRESCALE1 = Bitfield(13, 2);

    IopTimersUnitRegister_Mode();

    /// When written to, caches timer event source and enabled status (looks at IRQ conditions).
    /// Also resets the count register on write. Scope locked for the entire duration.
    void byte_bus_write_uhword(const BusContext context, const usize offset, const uhword value) override;
    void byte_bus_write_uword(const BusContext context, const usize offset, const uword value) override;

    /// Bus write latch. Signifies that the timer unit should be reset (ie: reset count with the prescale below).
    bool write_latch;

    /// Holds the cached result of if the timer is enabled, based on the interrupt bits set.
    /// Used as a way to increase performance by skipping over useless timers.
    bool is_enabled;

    /// Calculates unit parameters including:
    /// - Internally sets the event source this timer follows.
    /// - Returns the prescale that should be set on the count register.
    uword calculate_prescale_and_set_event(const int unit_id);

    /// Holds the cached result of which event type this timer is following.
    ControllerEventType event_type;
};