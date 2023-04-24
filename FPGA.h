#ifndef FPGA_OBFUSCATION_FPGA_H
#define FPGA_OBFUSCATION_FPGA_H

#include "LogicGate.h"

class FPGA
{
  private:
    std::vector<std::bitset<1>> _bitstream;
    std::vector<Port*> _input_pins;
    std::vector<Port*> _output_pins;
    std::vector<Port*> _ports;
    std::vector<Connection*> _connections;

    ///////////////////////////////////////////////////////////////
    ///                                                         ///
    /// Double vectors for parallelism.                         ///
    ///                                                         ///
    /// The idea is here is to process logic gates in parallel, ///
    /// hence why there is double vectors.                      ///
    /// Ideally, they represent different stages where we know  ///
    /// that each logic gates are not dependent on each others  ///
    ///                                                         ///
    /// Of course, this is not a requirement,                   ///
    /// one can just make it procedural                         ///
    ///                                                         ///
    ///////////////////////////////////////////////////////////////
    std::vector<std::vector<LogicGate*>> _logic_gates;

public:

};

#endif
