#include "Port.h"

using namespace FPGASimulator;

void Port::SetHigh()
{
    state = HIGH;
}

void Port::SetLow()
{
    state = LOW;
}
