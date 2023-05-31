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
        void AddVar(const T& value);
    };
}

template <typename T>
requires (VFPGAObfuscatorLibrary::GoodSerializedType<T>)
void VFPGAObfuscatorLibrary::Serializer::AddVar(const T& value)
{
    const auto InsertType = [&](SharedSerializedType type,
                                EncodedIndex sizeOfData,
                                const std::byte* byteValues)
    {
        ///////////////////
        /// Insert type ///
        ///////////////////
        data.push_back(static_cast<std::byte>(type));

        ///////////////////////////////////////////
        /// Ensure the size of the type or data ///
        ///////////////////////////////////////////
        const auto InsertData =
          [&](const std::byte* inputData, EncodedIndex size)
        {
            const auto dataSize = data.size();

            data.resize(dataSize + size);

            std::copy(inputData, inputData + size, &data[dataSize]);
        };

        InsertData(reinterpret_cast<const std::byte*>(&sizeOfData),
                   sizeof(sizeOfData));

        ////////////////////////////////
        /// Insert finally the value ///
        ////////////////////////////////
        InsertData(byteValues, sizeOfData);
    };

    if constexpr (std::is_integral_v<T>)
    {
        InsertType(SharedSerializedType::INTEGRAL,
                   sizeof(T),
                   reinterpret_cast<const std::byte*>(&value));
    }
    else
    {
        InsertType(SharedSerializedType::DATA,
                   value.size() * sizeof(std::byte),
                   value.data());
    }
}

#endif
