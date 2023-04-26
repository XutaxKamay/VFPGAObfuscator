#ifndef FPGA_SIMULATOR_LOGIC_GATE_H
#define FPGA_SIMULATOR_LOGIC_GATE_H

#include "Port.h"

class LogicGate
{
  public:
    struct Decoder
    {
        void RunLogicFunction(const std::vector<const Port*>& allPorts,
                              const std::vector<const Port*>& inputPorts,
                              const std::vector<Port*>& outputPorts);
    };

  protected:
    std::vector<const Port*> _all_ports;
    std::vector<const Port*> _input_ports;
    std::vector<Port*> _output_ports;
    Decoder _decoder;

  public:
    LogicGate(const std::vector<const Port*>& allPorts,
              const std::vector<const Port*>& inputPorts,
              const std::vector<Port*>& outputPorts,
              const Decoder& decoder);

    void Simulate();
};

#endif
