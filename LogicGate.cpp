#include "LogicGate.h"

LogicGate::LogicGate(std::array<Connection*, 2> inputConnection,
                     Connection* outputConnection,
                     logic_function_t logicFunction)
 : _input_connection { inputConnection },
   _output_connection { outputConnection },
   _logic_function(logicFunction)
{
}

void LogicGate::Simulate()
{
    _input_connection[0]->Synchronize();
    _input_connection[1]->Synchronize();

    _logic_function({ _input_connection[0]->GetOutputPort(),
                      _input_connection[1]->GetOutputPort() },
                    _output_connection->GetInputPort());
}
