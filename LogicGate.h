#ifndef FPGA_SIMULATOR_LOGIC_GATE_H
#define FPGA_SIMULATOR_LOGIC_GATE_H

#include "Port.h"

class LogicGate
{
  public:
    struct Decoded
    {
        void RunLogicFunction(const std::vector<Port*>& inputPorts,
                              const std::vector<Port*>& outputPorts);

        std::vector<std::vector<std::variant<Port*, Bit>>>
          input_truth_table;
        std::vector<std::variant<Port*, Bit>> output_truth_table;
    };

    struct Encoded
    {
        /*--------------------------------------------------
         |                                                 |
         |  The possible outputs for a port:               |
         |                                                 |
         |  An example would be like so:                   |
         |                                                 |
         |  INPUTS      OUTPUTS                            |
         |                                                 |
         |   A  B        O  Q                              |
         |  |----|      |----|                             |
         |  |0||0|      |0||1|                             |
         |  ------      ------                             |
         |  |0||1|      |0||0|                             |
         |  ------      ------                             |
         |  |1||0|      |1||0|                             |
         |  ------      ------                             |
         |  |1||1|      |1||1|                             |
         |  |----|      |----|                             |
         |                                                 |
         |  But it is also possible to make this           |
         |  and it would be also valid:                    |
         |                                                 |
         |                                                 |
         |  INPUTS      OUTPUTS                            |
         |                                                 |
         |   A  B        O  Q                              |
         |  |----|      |----|                             |
         |  |0||0|      |O||Q|                             |
         |  ------      ------                             |
         |  |0||1|      |Q||Q|                             |
         |  ------      ------                             |
         |  |1||0|      |0||1|                             |
         |  ------      ------                             |
         |  |1||1|      |1||0|                             |
         |  |----|      |-||-|                             |
         |                                                 |
         |  This is mostly useful for RAM,                 |
         |  like flip-flops or latchs.                     |
         |  Now the thing that is interesting              |
         |  and wouldn't be possible in                    |
         |  others circuits in physical world would be:    |
         |                                                 |
         |  INPUTS      OUTPUTS                            |
         |                                                 |
         |   A  B        O  Q                              |
         |  ------      ------                             |
         |  |0||1|      |Q||1|                             |
         |  ------      ------                             |
         |  |X||Y|      |0||1|                             |
         |  ------      ------                             |
         |  |1||1|      |1||0|                             |
         |  |-||-|      |-||-|                             |
         |                                                 |
         |  Where X and Y would be some unrelated ports    |
         |  that are not inside the inputs and outputs.    |
         |  Of course, a lot of combinations are missing   |
         |  here, but this is exactly the purpose.         |
         |  Those are not REAL logic gates.                |
         |  If combination is missing,                     |
         |  then it just skips without setting the output, |
         |  which makes it even more obfuscated            |
         |  and interesting.                               |
         |                                                 |
         --------------------------------------------------*/

        struct TruthTableElement
        {
            enum class type_t : std::uint8_t
            {
                PORT,
                BOOL,
            } type;
        };

        struct BoolInTruthTable : TruthTableElement
        {
            std::uint8_t value;
        };

        struct PortInTruthTable : TruthTableElement
        {
            encoded_index_t port_index;
        };

        encoded_index_t number_of_lines_in_truth_table;
        encoded_index_t number_of_input_ports;
        encoded_index_t number_of_output_ports;
        std::uint64_t offset_to_truth_table;
        std::uint64_t offset_to_first_input_port;
        std::uint64_t offset_to_first_output_port;

        std::vector<Port*> GetInputPorts(
          const std::vector<Port*>& allPorts);
        std::vector<Port*> GetOutputPorts(
          const std::vector<Port*>& allPorts);
        LogicGate::Decoded DecodeLogicFunction(
          const std::vector<Port*>& allPorts);
        LogicGate ToLogicGate(const std::vector<Port*>& allPorts);
    };

  protected:
    std::vector<Port*> _input_ports;
    std::vector<Port*> _output_ports;
    Decoded _decoded;

  public:
    LogicGate(const std::vector<Port*>& inputPorts,
              const std::vector<Port*>& outputPorts,
              const Decoded& decoded);

    void Simulate();
};

#endif
