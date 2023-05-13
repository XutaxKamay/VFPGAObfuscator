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

    template <typename T>
    requires (GoodSerializedType<T>)
    void Serializer::AddVar(const T& value)
    {
        const auto InsertData = [&](SharedSerializedType type,
                                    std::uint64_t sizeOfData,
                                    const auto byteValues)
        {
            ///////////////////
            /// Insert type ///
            ///////////////////
            data.push_back(static_cast<std::byte>(type));

            ///////////////////////////////////////////
            /// Ensure the size of the type or data ///
            ///////////////////////////////////////////
            data.insert(data.cend(),
                        reinterpret_cast<const std::byte*>(&sizeOfData),
                        reinterpret_cast<const std::byte*>(&sizeOfData)
                          + sizeof(sizeOfData));

            ////////////////////////////////
            /// Insert finally the value ///
            ////////////////////////////////
            data.insert(data.cend(),
                        reinterpret_cast<const std::byte*>(byteValues),
                        reinterpret_cast<const std::byte*>(byteValues)
                          + sizeOfData);
        };

        if constexpr (std::is_integral_v<T>)
        {
            InsertData(SharedSerializedType::INTEGRAL, sizeof(T), &value);
        }
        else
        {
            InsertData(SharedSerializedType::DATA,
                       value.size() * sizeof(std::byte),
                       value.data());
        }
    }
}

#endif
