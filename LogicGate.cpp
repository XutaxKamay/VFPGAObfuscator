#include "LogicGate.h"

void LogicGate::Decoder::RunLogicFunction(
  const std::vector<Port*>& inputPorts,
  const std::vector<Port*>& outputPorts)
{
    for (auto&& line : truth_table)
    {
    }
}

LogicGate::LogicGate(const std::vector<Port*>& inputPorts,
                     const std::vector<Port*>& outputPorts,
                     const Decoder& decoder)
 : _input_ports { inputPorts },
   _output_ports { outputPorts },
   _decoder { decoder }
{
}

void LogicGate::Simulate()
{
    _decoder.RunLogicFunction(_input_ports, _output_ports);
}
