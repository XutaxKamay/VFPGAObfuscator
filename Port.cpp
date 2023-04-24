#include "Port.h"

Port::Port(Bit&& state)
 : _state { state }
{
}

void Port::setHigh()
{
    _state = HIGH;
}

void Port::setLow()
{
    _state = LOW;
}

Bit& Port::state()
{
    return _state;
}
