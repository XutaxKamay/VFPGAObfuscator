#ifndef FPGA_SIMULATOR_SHARED_SERIALIZER_TYPE_H
#define FPGA_SIMULATOR_SHARED_SERIALIZER_TYPE_H

#include "StandardHeaders.h"

enum class SharedSerializedType : std::uint8_t
{
    INTEGRAL,
    DATA
};

template <typename T>
concept GoodSerializedType = std::is_integral_v<T>
                             or std::is_same_v<T, std::vector<std::byte>>;

#endif