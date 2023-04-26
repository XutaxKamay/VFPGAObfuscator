#include "LogicGate.h"

LogicGate::LogicGate(const std::vector<const Port*>& allPorts,
                     const std::vector<const Port*>& inputPorts,
                     const std::vector<Port*>& outputPorts,
                     const Decoder& decoder)
 : _all_ports { allPorts },
   _input_ports { inputPorts },
   _output_ports { outputPorts },
   _decoder { decoder }
{
}

void LogicGate::Simulate()
{
    _decoder.RunLogicFunction(_all_ports, _input_ports, _output_ports);
}
