#include "Connection.h"

Connection::Connection(Port* inputPort, Port* outputPort)
 : _input_port { inputPort },
   _output_port { outputPort }
{
}

Port* Connection::GetInputPort()
{
    return _input_port;
}

Port* Connection::GetOutputPort()
{
    return _output_port;
}

void Connection::Synchronize()
{
    _output_port->state() = _input_port->state();
}
