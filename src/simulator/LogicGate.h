#ifndef FPGA_SIMULATOR_LOGIC_GATE_H
#define FPGA_SIMULATOR_LOGIC_GATE_H

#include "Port.h"

/*--------------------HOW-IT-WORKS------------------
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
 |  |----|      |----|                             |
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
 |  |----|      |----|                             |
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
class LogicGate
{
  public:
    struct Decoder
    {
        void RunLogicFunction(const std::vector<Port*>& inputPorts,
                              const std::vector<Port*>& outputPorts);

        //////////////////////////////////////////////////////////
        /// Order is: [lines][column]                          ///
        ///                                                    ///
        /// The reason is for performance.                     ///
        /// While it may seem more logical to represent        ///
        /// the order in the other way, if you do it by first  ///
        /// selecting the column you have two algorithms:      ///
        ///                                                    ///
        /// One is going by iterating lines first,             ///
        /// but then you have CPU cache problems               ///
        /// and performance costs because                      ///
        /// you everytime you increase column index,           ///
        /// you essentially move on a whole new part of memory ///
        /// which isn't cache efficient (cache miss).          ///
        ///                                                    ///
        /// The second algorithm is more complicated,          ///
        /// you can start looping first with columns           ///
        /// and you won't have CPU cache problems              ///
        /// but you will need to keep track of the             ///
        /// line index and test again for all others columns   ///
        /// which is eventually slower than doing it           ///
        /// [lines][column].                                   ///
        //////////////////////////////////////////////////////////

        using ElementType = std::variant<Port*, Bit>;

        std::vector<std::vector<ElementType>> input_truth_table;
        std::vector<std::vector<ElementType>> output_truth_table;
    };

    /////////////
    /// TODO: ///
    /////////////
    struct Encoder
    {
    };

  protected:
    std::vector<Port*> _input_ports;
    std::vector<Port*> _output_ports;
    Decoder _decoder;

  public:
    LogicGate() = default;
    LogicGate(const std::vector<Port*>& inputPorts,
              const std::vector<Port*>& outputPorts,
              const Decoder& decoder);

    const decltype(_input_ports)& InputPorts() const;
    const decltype(_output_ports)& OutputPorts() const;
    const Decoder& Decoded() const;
    void Simulate();
};

#endif
