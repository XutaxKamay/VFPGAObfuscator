#ifndef FPGA_OBFUSCATION_LOGIC_GATE_H
#define FPGA_OBFUSCATION_LOGIC_GATE_H

#include "Port.h"

class LogicGate
{
    using logic_function_t = std::function<
      void(std::array<Port*, 2> inputPort, Port* outputPort)>;

  protected:
    std::array<Port*, 2> _input_port;
    Port* _output_port;
    logic_function_t _logic_function;

  public:
    LogicGate(std::array<Port*, 2> inputPort,
              Port* outputPort,
              logic_function_t logicFunction);

    void Simulate();
};

#endif
