#ifndef FPGA_SIMULATOR_FPGA_H
#define FPGA_SIMULATOR_FPGA_H

#include "Error.h"
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

namespace FPGASimulator
{
    class FPGA
    {
      private:
        struct Stage
        {
            std::vector<LogicGate> logic_gates;
            ///////////////////////////////////
            /// TODO:                       ///
            /// Maybe fill more info here,  ///
            /// like for multi-threading    ///
            ///////////////////////////////////
        };

      private:
        /////////////////////////////////////////
        /// Number of ports doesn't change.   ///
        /// We can still access to the vector ///
        /// elements through GetPort          ///
        /////////////////////////////////////////
        std::vector<Port> _ports;

        /////////////////////////////////////////////////
        /// Keep logic gates private,                 ///
        /// we don't want to set the vector directly, ///
        /// instead we insert the logic gates we want ///
        /// by design                                 ///
        /////////////////////////////////////////////////
        std::vector<LogicGate> _logic_gates;

        ///////////////////////////////////////////////////////////////
        ///                                                         ///
        /// The idea is here is to process logic gates in parallel, ///
        /// hence why there is double vectors.                      ///
        /// Ideally, they represent different stages where we know  ///
        /// that each logic gates are not dependent on each others  ///
        ///                                                         ///
        ///////////////////////////////////////////////////////////////
        std::vector<Stage> _stages;

      public:
        FPGA(const std::size_t numberOfPorts);

        Port* GetPort(std::size_t index);
        std::size_t NumberOfPorts() const;
        const std::vector<LogicGate>& LogicGates() const;

        void InsertLogicGate(const LogicGate& logicGate);

        void PrepareStages();
        void Simulate();

      private:
        ///////////////////////////////////////////////////////////////
        /// This should be compiled before,                         ///
        /// since this can be created before running the simulator. ///
        /// With a lot of logic gates, it may take a while.         ///
        ///////////////////////////////////////////////////////////////
        void CheckDependencyAndCreateStages();
    };
}

#endif
