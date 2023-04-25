#ifndef FPGA_OBFUSCATION_FPGA_H
#define FPGA_OBFUSCATION_FPGA_H

#include "LogicGate.h"

/*
                  FPGA
---------------------------------------
|                                     |
|     ██--|   CONNECTIONS   |--██     |
|         |   ===========   |         |
|     ██--|                 |--██     |
|         |                 |         |
|     ██--|    ---------    |--██     |
|         |    | LOGIC |    |         |
|     ██--|====|   -   |====|--██     |
|         |    | GATES |    |         |
|     ██--|    ---------    |--██     |
|         |                 |         |
|     ██--|                 |--██     |
|         |                 |         |
|     ██--|                 |--██     |
|                                     |
| INPUT PINS              OUTPUT PINS |
---------------------------------------


                    A Small Part Of Logic Hell Gates
     ---------------------------------------------------------------
     | STAGE X          ║ STAGE X+1 ║ STAGE X+2 ║        STAGE X+3 |
     |                  ║           ║           ║                  |
 (1)-|>========|        ║           ║           ║                  |
     |         |========$-|>[and]---$-|         ║        |=========|>-(1)
 (0)-|>========|        ║           ║ |         ║        |         |
     |                  ║           ║ |         ║        |=========|>-(1)
 (0)-|>========|        ║           ║ -|>[or]---$========|         |
     |         |========$-|>[or]--|-$-|         ║        |=========|>-(1)
 (1)-|>========|        ║         | ║           ║                  |
     |                  ║         | ║           ║                  |
     |                  ║         |-$-|         ║        |=========|>-(0)
 (1)-|>========|        ║           ║ |         ║        |         |
     |         |========$-|>[xor]-| ║ -|>[and]--$========|=========|>-(0)
 (1)-|>========|        ║         |-$-|         ║        |
     |                  ║           ║           ║        |=========|>-(0)
     |                  ║           ║           ║                  |
     | INPUT PORTS      ║           ║           ║     OUTPUT PORTS |
     ---------------------------------------------------------------


     In this scheme just above,
     the input ports represents
     the previous logic gates output ports.
     The '$' character is a signal that says
     that we can go onto the next stage.
     All '$' characters must be reached before
     going on another stage.
     This permits to get parallelism
     on our circuit since the logic gates
     in each stages can be processed seperately
     and are independent of each others.
     The stages can be generated automatically by looking
     at the connections,
     The more there is ouputs, the more parallelism we can get.
     Since our machine is virtual and there's no physical limitation,
     we can get a very high parallelism depending on the architecture.
*/

class FPGA
{
  public:
    struct Record
    {
        enum class type_t
        {
            INPUT_PIN,
            OUTPUT_PIN,
            PORT,
            CONNECTION,
            LOGIC_GATE
        };

        type_t type;
        std::size_t size;
    };

    struct PinRecord : Record
    {
        std::size_t index;
    };

    struct PortRecord : Record
    {
        std::size_t index;
    };

    struct ConnectionRecord : Record
    {
        std::array<std::size_t, 2> input_port_index;
        std::size_t output_port_index;
    };

    struct LogicGateRecord : Record
    {
        std::array<std::size_t, 2> input_connection_index;
        std::size_t output_connection_index;

        ////////////////////////////////////////////////////
        ///                                              ///
        /// 0 = first input port, 1 = second input port, ///
        /// 2 = output port                              ///
        ///                                              ///
        ////////////////////////////////////////////////////
        std::array<std::uint8_t, 3> truth_table_ports_column;
        std::array<std::array<bool, 3>, 4> truth_table;

        LogicGate ToLogicGate(std::array<Connection*, 2> inputConnection,
                              Connection* outputConnection);
    };

  private:
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
    FPGA(std::vector<std::byte> encodedBitStream);
};

#endif
