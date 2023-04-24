#include "LogicGate.h"

LogicGate::LogicGate(Connection* inputConnection1,
                     Connection* inputConnection2,
                     Connection* outputConnection)
 : _input_connection_1 { inputConnection1 },
   _input_connection_2 { inputConnection2 },
   _output_connection { outputConnection }
{
}

void LogicGate::Simulate(logic_function_t logicFunction)
{
    _input_connection_1->Synchronize();
    _input_connection_2->Synchronize();

    logicFunction(_input_connection_1->GetOutputPort(),
                  _input_connection_1->GetOutputPort(),
                  _output_connection->GetInputPort());
}
