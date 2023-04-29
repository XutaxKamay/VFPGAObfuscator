#ifndef FPGA_SIMULATOR_LOGIC_GATE_H
#define FPGA_SIMULATOR_LOGIC_GATE_H

#include "Port.h"

class LogicGate
{
  public:
    struct Decoder
    {
        void RunLogicFunction(const std::vector<Port*>& inputPorts,
                              const std::vector<Port*>& outputPorts);

        std::vector<std::vector<std::variant<Port*, Bit>>>
          input_truth_table;
        std::vector<std::variant<Port*, Bit>> output_truth_table;
    };

  protected:
    std::vector<Port*> _input_ports;
    std::vector<Port*> _output_ports;
    Decoder _decoder;

  public:
    LogicGate(const std::vector<Port*>& inputPorts,
              const std::vector<Port*>& outputPorts,
              const Decoder& decoder);

    void Simulate();
};

#endif
