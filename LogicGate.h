#ifndef FPGA_OBFUSCATION_LOGIC_GATE_H
#define FPGA_OBFUSCATION_LOGIC_GATE_H

#include "Connection.h"

class LogicGate
{
    using logic_function_t = std::function<
      void(Port* inputPort1, Port* inputPort2, Port* outputPort)>;

  protected:
    Connection* _input_connection_1;
    Connection* _input_connection_2;
    Connection* _output_connection;

  public:
    LogicGate(Connection* inputConnection1,
              Connection* inputConnection2,
              Connection* outputConnection);

    void Simulate(logic_function_t logicFunction);
};

#endif
