#pragma once

#include <cereal/cereal.hpp>

#include "Common/Types/Memory/ArrayByteMemory.hpp"
#include "Common/Types/Register/SizedWordRegister.hpp"
#include "Resources/Ee/Gif/GifRegisters.hpp"

class RGif
{
public:
    RGif();

    /// GIF memory mapped registers. See page 21 of EE Users Manual.
    GifRegister_Ctrl ctrl;
    GifRegister_Mode mode;
    GifRegister_Stat stat;
    ArrayByteMemory memory_3030;
    GifRegister_Tag0 tag0;
    GifRegister_Tag1 tag1;
    GifRegister_Tag2 tag2;
    GifRegister_Tag3 tag3;
    GifRegister_Cnt cnt;
    GifRegister_P3cnt p3cnt;
    GifRegister_P3tag p3tag;
    ArrayByteMemory memory_30b0;

public:
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(ctrl),
            CEREAL_NVP(mode),
            CEREAL_NVP(stat),
            CEREAL_NVP(memory_3030),
            CEREAL_NVP(tag0),
            CEREAL_NVP(tag1),
            CEREAL_NVP(tag2),
            CEREAL_NVP(tag3),
            CEREAL_NVP(cnt),
            CEREAL_NVP(p3cnt),
            CEREAL_NVP(p3tag),
            CEREAL_NVP(memory_30b0));
    }
};
