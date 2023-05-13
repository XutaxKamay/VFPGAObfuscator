#include "Deserializer.h"

using namespace VFPGAObfuscatorLibrary;

bool Deserializer::CanReadVar(std::size_t size)
{
    return (data_index + size) <= data.size();
}
