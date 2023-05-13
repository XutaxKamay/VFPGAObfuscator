#include "Deserializer.h"

using namespace FPGAObfuscatorLibrary;

bool Deserializer::CanReadVar(std::size_t size)
{
    return (data_index + size) <= data.size();
}
