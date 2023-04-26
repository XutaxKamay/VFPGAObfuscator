#ifndef FPGA_SIMULATOR_WIRE_H
#define FPGA_SIMULATOR_WIRE_H

class LogicGate
{
  public:
    virtual ~LogicGate()    = 0;
    virtual void Simulate() = 0;
};

#endif
