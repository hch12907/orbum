#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

#include "Common/Types/FifoQueue/DmaFifoQueue.hpp"
#include "Common/Types/Register/ByteRegister.hpp"
#include "Common/Types/Register/SizedWordRegister.hpp"
#include "Common/Types/ScopeLock.hpp"

/// SIO2 CTRL Register.
/// Bits 0, 2, 3 appear to be write only that toggle the send or receive mode.
/// Not sure about bit 1, but assuming it is also a write only bit (not used
/// elsewhere). If bits 2 & 3 are set (ie: byte 0xC written), this is TX
/// (to SIO0) mode. If bit 0 is set (ie: byte 0x1 or 0xD written), this is RX
/// (from SIO0) mode. 0xC, 0x1 and 0xD are the normal values written by the IOP.
class Sio2Register_Ctrl : public SizedWordRegister, public ScopeLock
{
public:
    /// SIO2 transfer direction to/from SIO0.
    enum class Direction
    {
        TX,
        RX,
    };

    static constexpr Bitfield DIRECTION = Bitfield(0, 4); // Direction bits - not exactly sure what every bit means.
                                                          // One of the bits is probably the reset SIO0 bit.

    Sio2Register_Ctrl();

    /// Scope locked for entire duration.
    /// Sets the write latch on bus write, throws error when it has already been set.
    void byte_bus_write_uword(const BusContext context, const usize offset, const uword value) override;

    /// Extracts the set direction from the register.
    Direction get_direction();

    /// Current transfer status to/from SIO0.
    bool transfer_started;
    size_t transfer_port;
    size_t transfer_port_count;
    Direction transfer_direction;

    /// Write latch, set to true on bus write, cleared by the controller.
    bool write_latch;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::base_class<SizedWordRegister>(this),
            CEREAL_NVP(transfer_started),
            CEREAL_NVP(transfer_port),
            CEREAL_NVP(transfer_port_count),
            CEREAL_NVP(transfer_direction),
            CEREAL_NVP(write_latch));
    }
};
