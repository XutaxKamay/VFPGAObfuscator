#include "Deserializer.h"

Deserializer::Deserializer(const std::vector<std::byte>& data)
 : _data { data },
   _data_index {}
{
}

bool Deserializer::CanReadVar(std::size_t size)
{
    return (_data_index + size) <= _data.size();
}

const decltype(Deserializer::_data)& Deserializer::GetData() const
{
    return _data;
}
