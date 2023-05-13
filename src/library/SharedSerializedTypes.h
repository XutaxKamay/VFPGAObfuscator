#ifndef VFPGA_OBFUSCATOR_LIBRARY_SHARED_SERIALIZER_TYPES_H
#define VFPGA_OBFUSCATOR_LIBRARY_SHARED_SERIALIZER_TYPES_H

#include "StandardHeaders.h"

namespace VFPGAObfuscatorLibrary
{
    enum class SharedSerializedType : std::uint8_t
    {
        INTEGRAL,
        DATA
    };

    template <typename T>
    concept GoodSerializedType = std::is_integral_v<T>
                                 or std::is_same_v<T,
                                                   std::vector<std::byte>>;
}

#endif
