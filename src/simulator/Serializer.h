#ifndef FPGA_SIMULATOR_SERIALIZER_H
#define FPGA_SIMULATOR_SERIALIZER_H

#include "SharedSerializedTypes.h"

class Serializer
{
  private:
    std::vector<std::byte> _data;

  public:
    template <typename T>
    requires (GoodSerializedType<T>)
    void AddVar(const T& value);

    const decltype(_data)& GetData() const;
};

template <typename T>
requires (GoodSerializedType<T>)
void Serializer::AddVar(const T& value)
{
    SharedSerializedType type;
    std::uint64_t sizeOfData;

    static const auto InsertData = [&](auto&& byteValues)
    {
        ///////////////////
        /// Insert type ///
        ///////////////////
        _data.push_back(static_cast<std::byte>(type));

        ///////////////////////////////////////////
        /// Ensure the size of the type or data ///
        ///////////////////////////////////////////
        _data.insert(_data.cend(),
                     reinterpret_cast<const std::byte*>(&sizeOfData),
                     reinterpret_cast<const std::byte*>(&sizeOfData)
                       + sizeof(sizeOfData));

	////////////////////////////////
	/// Insert finally the value ///
	////////////////////////////////
        _data.insert(_data.cend(),
                     reinterpret_cast<const std::byte*>(byteValues),
                     reinterpret_cast<const std::byte*>(byteValues)
                       + sizeOfData);
    };

    if constexpr (std::is_integral_v<T>)
    {
        type       = SharedSerializedType::INTEGRAL;
        sizeOfData = sizeof(T);

        InsertData(&value);
    }
    else
    {
        type       = SharedSerializedType::DATA;
        sizeOfData = value.size() * sizeof(std::byte);

        InsertData(value.data());
    }
}

#endif
