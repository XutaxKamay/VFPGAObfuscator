#include "Port.h"

using namespace VFPGAObfuscatorSimulator;

void Port::SetHigh()
{
    state = HIGH;
}

void Port::SetLow()
{
    state = LOW;
}
