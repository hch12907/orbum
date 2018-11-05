#pragma once

#include <stdexcept>

#include <cereal/cereal.hpp>

#include "Common/Types/Primitive.hpp"
#include "Common/Types/Register/HwordRegister.hpp"

/// Sized Hword register.
class SizedHwordRegister : public HwordRegister
{
public:
    SizedHwordRegister(const uhword initial_value = 0, const bool read_only = false) :
        h(initial_value),
        initial_value(initial_value),
        read_only(read_only)
    {
    }

    /// Initialise register.
    void initialize() override
    {
        h = initial_value;
    }

    /// Read/write functions to access the register.
    ubyte read_ubyte(const size_t offset) const override
    {
#if defined(BUILD_DEBUG)
        if (offset >= NUMBER_BYTES_IN_HWORD)
            throw std::runtime_error("Tried to access SizedHwordRegister with an invalid offset.");
#endif

        return b[offset];
    }

    void write_ubyte(const size_t offset, const ubyte value) override
    {
#if defined(BUILD_DEBUG)
        if (offset >= NUMBER_BYTES_IN_HWORD)
            throw std::runtime_error("Tried to access SizedHwordRegister with an invalid offset.");
#endif

        if (!read_only)
            b[offset] = value;
    }

    uhword read_uhword() const override
    {
        return h;
    }

    void write_uhword(const uhword value) override
    {
        if (!read_only)
            h = value;
    }

private:
    /// Primitive (sized) storage for register.
    union {
        ubyte b[NUMBER_BYTES_IN_HWORD];
        uhword h;
    };

    /// Initial value.
    uhword initial_value;

    /// Read-only flag.
    /// Writes are silently discarded if turned on.
    bool read_only;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(h));
    }
};
