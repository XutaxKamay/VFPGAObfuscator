#include "Serializer.h"

const decltype(Serializer::_data)& Serializer::GetData() const
{
    return _data;
}
