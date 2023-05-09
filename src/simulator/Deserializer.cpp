#include "Deserializer.h"

using namespace FPGASimulator;

bool Deserializer::CanReadVar(std::size_t size)
{
    return (data_index + size) <= data.size();
}
