#include "LogicGate.h"

LogicGate::LogicGate(std::array<Port*, 2> inputPort,
                     Port* outputPort,
                     logic_function_t logicFunction)
 : _input_port { inputPort },
   _output_port { outputPort },
   _logic_function(logicFunction)
{
}

void LogicGate::Simulate()
{
    _logic_function({ _input_port[0], _input_port[1] }, _output_port);
}
