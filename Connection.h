#ifndef FPGA_OBFUSCATION_LOGIC_CONNECTION_H
#define FPGA_OBFUSCATION_LOGIC_CONNECTION_H

#include "Port.h"

struct Connection
{
    friend class LogicGate;

  private:
    Port* _input_port;
    Port* _output_port;

  public:
    Connection(Port* inputPort, Port* outputPort);

  protected:
    Port* GetInputPort();
    Port* GetOutputPort();
    void Synchronize();
};

#endif
