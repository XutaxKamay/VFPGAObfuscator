#ifndef VFPGA_OBFUSCATOR_LIBRARY_DESERIALIZER_H
#define VFPGA_OBFUSCATOR_LIBRARY_DESERIALIZER_H

#include "Error.h"
#include "SharedSerializedTypes.h"

namespace VFPGAObfuscatorLibrary
{
    class Deserializer
    {
      public:
        enum class ReadStatus
        {
            NO_ERROR,
            NOT_SAME_TYPE,
            SIZE_OF_INTEGRAL_NOT_CORRECT,
            OUT_OF_BOUNDS
        };

        std::vector<std::byte> data;
        std::size_t data_index {};

        template <typename T>
        requires (GoodSerializedType<T>)
        T ReadVar(ReadStatus* readStatus = nullptr);

        template <typename T>
        auto ReadAndCheckStatus();

      private:
        bool CanReadVar(std::size_t size);

        template <typename T>
        auto ReadType();
    };
}

template <typename T>
auto VFPGAObfuscatorLibrary::Deserializer::ReadType()
{
    T var;

    std::copy(&data[data_index],
              &data[data_index + sizeof(T)],
              reinterpret_cast<std::byte*>(&var));

    data_index += sizeof(T);

    return var;
}

template <typename T>
requires (VFPGAObfuscatorLibrary::GoodSerializedType<T>)
T VFPGAObfuscatorLibrary::Deserializer::ReadVar(ReadStatus* readStatus)
{
    T value {};

    const auto SetStatusAndReturn = [&]<ReadStatus READ_STATUS>()
    {
        if (readStatus)
        {
            *readStatus = READ_STATUS;
        }

        return value;
    };

    if (not CanReadVar(sizeof(SharedSerializedType)
                       + sizeof(EncodedIndex)))
    {
        return SetStatusAndReturn
          .template operator()<ReadStatus::OUT_OF_BOUNDS>();
    }

    const auto type       = ReadType<SharedSerializedType>();
    const auto sizeOfData = ReadType<EncodedIndex>();

    if (not CanReadVar(sizeOfData))
    {
        return SetStatusAndReturn
          .template operator()<ReadStatus::OUT_OF_BOUNDS>();
    }

    if constexpr (std::is_integral_v<T>)
    {
        if (type != SharedSerializedType::INTEGRAL)
        {
            return SetStatusAndReturn
              .template operator()<ReadStatus::NOT_SAME_TYPE>();
        }

        if (sizeOfData != sizeof(T))
        {
            return SetStatusAndReturn.template
              operator()<ReadStatus::SIZE_OF_INTEGRAL_NOT_CORRECT>();
        }

        value = ReadType<T>();
    }
    else
    {
        if (type != SharedSerializedType::DATA)
        {
            return SetStatusAndReturn
              .template operator()<ReadStatus::NOT_SAME_TYPE>();
        }

        value = T { &data[data_index], &data[data_index] + sizeOfData };

        data_index += sizeOfData;
    }

    return SetStatusAndReturn.template operator()<ReadStatus::NO_ERROR>();
}

template <typename T>
auto VFPGAObfuscatorLibrary::Deserializer::ReadAndCheckStatus()
{
    auto readStatus = ReadStatus::NO_ERROR;
    auto var        = ReadVar<T>(&readStatus);

    if (readStatus != ReadStatus::NO_ERROR)
    {
        Error::ExitWithMsg(Error::Msg::DESERIALIZER_READ_FAILED);
    }

    return var;
}

#endif
