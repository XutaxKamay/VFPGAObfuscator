#ifndef FPGA_SIMULATOR_DESERIALIZER_H
#define FPGA_SIMULATOR_DESERIALIZER_H

#include "SharedSerializedTypes.h"

namespace FPGASimulator
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
        T ReadVar(std::optional<ReadStatus> status = std::nullopt);

      private:
        bool CanReadVar(std::size_t size);

        template <typename T>
        auto ReadType();
    };

    template <typename T>
    auto Deserializer::ReadType()
    {
        const auto var = *reinterpret_cast<T*>(&data[data_index]);

        data_index += sizeof(T);

        return var;
    }

    template <typename T>
    requires (GoodSerializedType<T>)
    T Deserializer::ReadVar(std::optional<ReadStatus> status)
    {
        T value {};

        if (not CanReadVar(sizeof(SharedSerializedType)
                           + sizeof(std::uint64_t)))
        {
            if (status)
            {
                status = ReadStatus::OUT_OF_BOUNDS;
            }

            return value;
        }

        const auto type       = ReadType<SharedSerializedType>();
        const auto sizeOfData = ReadType<std::uint64_t>();

        if (not CanReadVar(sizeOfData))
        {
            if (status)
            {
                status = ReadStatus::OUT_OF_BOUNDS;
            }

            return value;
        }

        if constexpr (std::is_integral_v<T>)
        {
            if (type != SharedSerializedType::INTEGRAL)
            {
                if (status)
                {
                    status = ReadStatus::NOT_SAME_TYPE;
                }

                return value;
            }

            if (sizeOfData != sizeof(T))
            {
                if (status)
                {
                    status = ReadStatus::SIZE_OF_INTEGRAL_NOT_CORRECT;
                }

                return value;
            }

            value = ReadType<T>();
        }
        else
        {
            if (type != SharedSerializedType::DATA)
            {
                if (status)
                {
                    status = ReadStatus::NOT_SAME_TYPE;
                }

                return value;
            }

            value = T { &data[data_index],
                        &data[data_index] + sizeOfData };

            data_index += sizeOfData;
        }

        if (status)
        {
            status = ReadStatus::NO_ERROR;
        }

        return value;
    }
}

#endif
