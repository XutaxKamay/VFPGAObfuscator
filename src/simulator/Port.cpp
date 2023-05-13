#include "Port.h"

using namespace FPGAObfuscatorSimulator;

void Port::SetHigh()
{
    state = HIGH;
}

void Port::SetLow()
{
    state = LOW;
}
