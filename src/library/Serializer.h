#ifndef VFPGA_OBFUSCATOR_LIBRARY_SERIALIZER_H
#define VFPGA_OBFUSCATOR_LIBRARY_SERIALIZER_H

#include "SharedSerializedTypes.h"

namespace VFPGAObfuscatorLibrary
{
    class Serializer
    {
      public:
        std::vector<std::byte> data;

        template <typename T>
        requires (GoodSerializedType<T>)
        constexpr void AddVar(const T& value);
    };
}

template <typename T>
requires (VFPGAObfuscatorLibrary::GoodSerializedType<T>)
constexpr void VFPGAObfuscatorLibrary::Serializer::AddVar(const T& value)
{
    const auto InsertData =
      [&](SharedSerializedType type, EncodedIndex sizeOfData)
    {
        ///////////////////
        /// Insert type ///
        ///////////////////
        data.push_back(static_cast<std::byte>(type));

        ///////////////////////////////////////////
        /// Ensure the size of the type or data ///
        ///////////////////////////////////////////
        for (std::size_t i = 0; i < sizeof(sizeOfData); i++)
        {
            data.push_back(static_cast<std::byte>(
              (sizeOfData >> (i * CHAR_BIT)) & 0xFF));
        }
    };

    if constexpr (std::is_integral_v<T>)
    {
        InsertData(SharedSerializedType::INTEGRAL, sizeof(T));

        ////////////////////////////////
        /// Insert finally the value ///
        ////////////////////////////////
        for (std::size_t i = 0; i < sizeof(T); i++)
        {
            data.push_back(
              static_cast<std::byte>((value >> (i * CHAR_BIT)) & 0xFF));
        }
    }
    else
    {
        InsertData(SharedSerializedType::DATA, value.size());

        ////////////////////////////////
        /// Insert finally the value ///
        ////////////////////////////////
        data.insert(data.cend(), value.begin(), value.end());
    }
}

#endif
