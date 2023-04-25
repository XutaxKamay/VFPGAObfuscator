#ifndef FPGA_OBFUSCATION_LOGIC_GATE_H
#define FPGA_OBFUSCATION_LOGIC_GATE_H

#include "Connection.h"

class LogicGate
{
    using logic_function_t = std::function<
      void(std::array<Port*, 2> inputPort, Port* outputPort)>;

  protected:
    std::array<Connection*, 2> _input_connection;
    Connection* _output_connection;
    logic_function_t _logic_function;

  public:
    LogicGate(std::array<Connection*, 2> inputConnection,
              Connection* outputConnection,
              logic_function_t logicFunction);

    void Simulate();
};

#endif
