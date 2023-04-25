#include "Port.h"

Port::Port(Bit&& state)
 : _state { state }
{
}

void Port::SetHigh()
{
    _state = HIGH;
}

void Port::SetLow()
{
    _state = LOW;
}

Bit Port::GetState()
{
    return _state;
}

void Port::SetState(Bit state)
{
    _state = state;
}
