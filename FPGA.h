#ifndef FPGA_SIMULATOR_FPGA_H
#define FPGA_SIMULATOR_FPGA_H

#include "LogicGate.h"

/*-----------------------------------------------------------------------
 |                                                                      |
 |                                FPGA                                  |
 |               ---------------------------------------                |
 |               |                                     |                |
 |               |     ██--|   CONNECTIONS   |--██     |                |
 |               |         |   ===========   |         |                |
 |               |     ██--|                 |--██     |                |
 |               |         |                 |         |                |
 |               |     ██--|    ---------    |--██     |                |
 |               |         |    | LOGIC |    |         |                |
 |               |     ██--|====|   -   |====|--██     |                |
 |               |         |    | GATES |    |         |                |
 |               |     ██--|    ---------    |--██     |                |
 |               |         |                 |         |                |
 |               |     ██--|                 |--██     |                |
 |               |         |                 |         |                |
 |               |     ██--|                 |--██     |                |
 |               |                                     |                |
 |               | INPUT PINS              OUTPUT PINS |                |
 |               ---------------------------------------                |
 |                                                                      |
 |                                                                      |
 |                    A Small Part Of Logic Hell Gates                  |
 |      ---------------------------------------------------------       |
 |      | STAGE X       ║ STAGE X+1 ║ STAGE X+2 ║     STAGE X+3 |       |
 |      |               ║           ║           ║               |       |
 |  (1)-|>========|     ║           ║           ║               |       |
 |      |         |=====$-|>[and]---$-|         ║     |=========|>-(1)  |
 |  (0)-|>========|     ║           ║ |         ║     |         |       |
 |      |               ║           ║ |         ║     |=========|>-(1)  |
 |  (0)-|>========|     ║           ║ -|>[or]---$=====|         |       |
 |      |         |=====$-|>[or]--|-$-|         ║     |=========|>-(1)  |
 |  (1)-|>========|     ║         | ║           ║               |       |
 |      |               ║         | ║           ║               |       |
 |      |               ║         |-$-|         ║     |=========|>-(0)  |
 |  (1)-|>========|     ║           ║ |         ║     |         |       |
 |      |         |=====$-|>[xor]-| ║ -|>[and]--$=====|=========|>-(0)  |
 |  (1)-|>========|     ║         |-$-|         ║     |                 |
 |      |               ║           ║           ║     |=========|>-(0)  |
 |      |               ║           ║           ║               |       |
 |      | INPUT PORTS   ║           ║           ║  OUTPUT PORTS |       |
 |      ---------------------------------------------------------       |
 |                                                                      |
 |                                                                      |
 |      In this scheme just above,                                      |
 |      the input ports represents                                      |
 |      the previous logic gates output ports.                          |
 |      The '$' character is a signal that says                         |
 |      that we can go onto the next stage.                             |
 |      All '$' characters must be reached before                       |
 |      going on another stage.                                         |
 |      This permits to get parallelism                                 |
 |      on our circuit since the logic gates                            |
 |      in each stages can be processed seperately                      |
 |      and are independent of each others.                             |
 |      The stages can be generated automatically by looking            |
 |      at the ports,                                                   |
 |      The more there is ports, the more parallelism we can get.       |
 |      Since our machine is virtual and                                |
 |      there's no physical limitation,                                 |
 |      we can get a very high parallelism                              |
 |      depending on the architecture.                                  |
 |                                                                      |
 -----------------------------------------------------------------------*/

class FPGA
{
    ////////////////////////////////////////////////////
    /// A pin is also a port, just a pin is exposed, ///
    /// a port may be not exposed                    ///
    ////////////////////////////////////////////////////
    using Pin     = Port;
    using index_t = std::uint64_t;

    struct Record
    {
        enum class type_t : std::uint8_t
        {
            ALL_PINS_AND_PORTS,
            LOGIC_GATE
        };

        type_t type;
        index_t size;
    };

    struct AllPinsAndPortsRecord : Record
    {
        index_t number_of_input_pins;
        index_t number_of_output_pins;
        index_t number_of_ports;

        /////////////////////////////////////////////
        /// input pins, outout pins, others ports ///
        /////////////////////////////////////////////
        using return_t = std::tuple<std::vector<FPGA::Pin*>,
                                    std::vector<FPGA::Pin*>,
                                    std::vector<Port*>>;

        return_t Generate();
    };

    struct PortsIndexesRecord : Record
    {
        index_t number_of_ports_indexes;
        index_t first_port_index;

        std::vector<index_t> Read();
    };

    struct LogicGateRecord : Record
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
            index_t port_index;
        };

        index_t number_of_lines_in_truth_table;
        index_t number_of_input_ports;
        index_t number_of_output_ports;
        std::uint64_t offset_to_truth_table;
        std::uint64_t offset_to_first_input_port;
        std::uint64_t offset_to_first_output_port;

        std::vector<Port*> GetInputPorts(
          const std::vector<Port*>& allPorts);
        std::vector<Port*> GetOutputPorts(
          const std::vector<Port*>& allPorts);
        LogicGate::Decoder DecodeLogicFunction(
          const std::vector<Port*>& allPorts);
        LogicGate ToLogicGate(const std::vector<Port*>& allPorts);
    };

  private:
    std::vector<Pin*> _input_pins;
    std::vector<Pin*> _output_pins;
    std::vector<Port*> _ports;

    ///////////////////////////////////////////////////////////////
    ///                                                         ///
    /// Double vectors for parallelism.                         ///
    ///                                                         ///
    /// The idea is here is to process logic gates in parallel, ///
    /// hence why there is double vectors.                      ///
    /// Ideally, they represent different stages where we know  ///
    /// that each logic gates are not dependent on each others  ///
    ///                                                         ///
    ///////////////////////////////////////////////////////////////
    std::vector<std::vector<LogicGate*>> _logic_gates;

  public:
    FPGA(const std::vector<std::byte>& encodedBitStream);
};

#endif
