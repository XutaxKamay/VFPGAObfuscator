#ifndef VFPGA_OBFUSCATOR_SIMULATOR_VFPGA_H
#define VFPGA_OBFUSCATOR_SIMULATOR_VFPGA_H

#include "LogicGate.h"
#include "Serializer.h"

/*-----------------------------------------------------------------------
 |                                                                      |
 |                                VFPGA                                 |
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

namespace VFPGAObfuscatorSimulator
{
    class VFPGA
    {
      public:
        using PortIndex = LogicGate::Serializer::Port;
        using PortInit  = std::tuple<VFPGAObfuscatorLibrary::EncodedIndex,
                                    VFPGAObfuscatorLibrary::Bit>;

        struct Deserializer
        {
            VFPGA Deserialize(const std::vector<std::byte>& serialized);
        };

        struct Serializer
        {
            struct Stage
            {
                std::vector<LogicGate::Serializer> logic_gates;
            };

            VFPGAObfuscatorLibrary::EncodedIndex number_of_ports;
            std::vector<LogicGate::Serializer> logic_gates;
            std::vector<PortInit> ports_init;

            template <bool PREPARE_STAGES>
            constexpr std::vector<std::byte> Serialize();
        };

      public:
        struct Stage
        {
            std::vector<LogicGate> logic_gates;
            ///////////////////////////////////
            /// TODO:                       ///
            /// Maybe fill more info here,  ///
            /// like for multi-threading    ///
            ///////////////////////////////////
        };

        ///////////////////////////////////////////////////////////////
        ///                                                         ///
        /// The idea is here is to process logic gates in parallel, ///
        /// hence why there is double vectors.                      ///
        /// Ideally, they represent different stages where we know  ///
        /// that each logic gates are not dependent on each others  ///
        ///                                                         ///
        ///////////////////////////////////////////////////////////////
        std::vector<Stage> stages;

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

      public:
        VFPGA(const std::size_t numberOfPorts);

        Port* GetPort(std::size_t index);
        std::size_t NumberOfPorts() const;
        const std::vector<LogicGate>& LogicGates() const
          __attribute__((const));

        void CheckLogicGateValidity(const LogicGate& logicGate);
        void InsertLogicGate(const LogicGate& logicGate);
        void InsertLogicGates(const std::vector<LogicGate>& logicGates);

        void PrepareStages();
        void Simulate();

      private:
        ///////////////////////////////////////////////////////////////
        /// This should be compiled before,                         ///
        /// since this can be created before running the simulator. ///
        /// With a lot of logic gates, it may take a while.         ///
        ///////////////////////////////////////////////////////////////
        template <class LOGIC_GATE_T, class PORT_T, class STAGE_T>
        static constexpr std::vector<STAGE_T> CheckDependencyAndCreateStages(
          const std::vector<LOGIC_GATE_T>& logicGates);
    };

}

template <bool PREPARE_STAGES>
constexpr std::vector<std::byte> VFPGAObfuscatorSimulator::VFPGA::
  Serializer::Serialize()
{
    VFPGAObfuscatorLibrary::Serializer serializer;

    serializer.AddVar(PREPARE_STAGES);
    serializer.AddVar<VFPGAObfuscatorLibrary::EncodedIndex>(
      number_of_ports);

    const auto AddLogicGates =
      [&](const decltype(logic_gates)& logicGatesSerializer)
    {
        std::ranges::for_each(
          logicGatesSerializer,
          [&](const LogicGate::Serializer& logicGateSerializer)
          {
              serializer.AddVar(logicGateSerializer.Serialize());
          });
    };

    if constexpr (PREPARE_STAGES)
    {
        auto preparedStages = CheckDependencyAndCreateStages<
          LogicGate::Serializer,
          VFPGAObfuscatorLibrary::EncodedIndex,
          Stage>(logic_gates);

        serializer.AddVar<VFPGAObfuscatorLibrary::EncodedIndex>(
          preparedStages.size());

        std::ranges::for_each(
          preparedStages,
          [&](const Stage& stage)
          {
              serializer.AddVar<VFPGAObfuscatorLibrary::EncodedIndex>(
                stage.logic_gates.size());
              AddLogicGates(stage.logic_gates);
          });
    }
    else
    {
        serializer.AddVar<VFPGAObfuscatorLibrary::EncodedIndex>(
          logic_gates.size());
        AddLogicGates(logic_gates);
    }

    serializer.AddVar<VFPGAObfuscatorLibrary::EncodedIndex>(
      ports_init.size());

    std::ranges::for_each(ports_init.begin(),
                          ports_init.end(),
                          [&](const PortInit& portInit)
                          {
                              serializer.AddVar(std::get<0>(portInit));
                              serializer.AddVar(
                                std::get<1>(portInit).value);
                          });

    return serializer.data;
}

template <class LOGIC_GATE_T, class PORT_T, class STAGE_T>
constexpr std::vector<STAGE_T> VFPGAObfuscatorSimulator::VFPGA::
  CheckDependencyAndCreateStages(
    const std::vector<LOGIC_GATE_T>& logicGates)
{
    ////////////////////////////////////////////
    /// Each stages will be represented as a ///
    /// vector of LOGIC_GATE_T               ///
    ////////////////////////////////////////////
    std::vector<STAGE_T> stages;
    std::vector<LOGIC_GATE_T> logicGatesLeft = logicGates;

    ///////////////////////////////////
    /// We can get it by reference, ///
    /// it's safe enough here       ///
    ///////////////////////////////////
    std::vector<LOGIC_GATE_T*> currentLogicGates;
    std::vector<PORT_T> currentOutputPorts;

    /////////////////////////
    /// Reserve memory    ///
    /// for faster pushes ///
    /////////////////////////
    const auto ReserveMemory = [&]()
    {
        stages.reserve(logicGatesLeft.size());
        currentLogicGates.reserve(logicGatesLeft.size());

        std::size_t maxNumberOfOutputPorts = 0;

        std::ranges::for_each(
          logicGates,
          [&](const LOGIC_GATE_T& logicGate) noexcept
          {
              maxNumberOfOutputPorts += logicGate.output_ports.size();
          });

        currentOutputPorts.reserve(maxNumberOfOutputPorts);
    };

    ReserveMemory();

    ////////////////////////////////////////////////////////////////////
    /// Each stages will contain logic gates that are not depending  ///
    /// on each others which is useful for parallelism.              ///
    ///                                                              ///
    /// In order to divide the logic gates in multiple stages,       ///
    /// we need to check if the previous logic gates                 ///
    /// has outputs has links/connections                            ///
    /// to the next logic gates inputs.                              ///
    ///                                                              ///
    /// If any previous logic gates we registered in an array        ///
    /// have links/connections to the next ones,                     ///
    /// then we ignore the next logic gate                           ///
    /// and we go on again on another logic gate                     ///
    /// until we traversed all the possible logic gates.             ///
    ///                                                              ///
    /// If not, we keep track of that next logic gate                ///
    /// inside an array and we keep searching for links/connections. ///
    ///                                                              ///
    /// This permits to simulate logic gates fully in parallel       ///
    /// (without locks) and stages themselves (with locks).          ///
    ////////////////////////////////////////////////////////////////////
    while (logicGatesLeft.size())
    {
        /////////////////////////////////////////////////////////////
        /// Check if any previous logic gates have outputs ports  ///
        /// linked to the logic gates left in their inputs ports. ///
        /////////////////////////////////////////////////////////////
        std::ranges::for_each(
          logicGatesLeft,
          [&](LOGIC_GATE_T& logicGate)
          {
              const auto foundPort = std::ranges::find_if(
                logicGate.input_ports,
                [&](PORT_T inputPort)
                {
                    //////////////////////////////////
                    /// Check if there's any links ///
                    //////////////////////////////////
                    return std::ranges::find(currentOutputPorts,
                                             inputPort)
                           != currentOutputPorts.end();
                });

              ////////////////////////////////////////
              /// We need to add all output ports, ///
              /// even if we didn't find a link    ///
              /// since the output ports           ///
              /// are dependent of the previous    ///
              /// output ports of the previous     ///
              /// logic gate                       ///
              ////////////////////////////////////////
              currentOutputPorts.insert(currentOutputPorts.begin(),
                                        logicGate.output_ports.begin(),
                                        logicGate.output_ports.end());

              /////////////////////////////////////////////////
              /// If we found a previous                    ///
              /// logic gates output port linked            ///
              /// to an input port in the logic gates left, ///
              /// We simply skip it for another stage.      ///
              /// If not found, we insert the current       ///
              /// logic gate inside the array               ///
              /////////////////////////////////////////////////
              if (foundPort == logicGate.input_ports.end())
              {
                  currentLogicGates.push_back(&logicGate);
              }
          });

        ////////////////////////////////////////////////////////////////
        /// Push back the array inside a new stage,                  ///
        /// And process again all the ignored logic gates previously ///
        ////////////////////////////////////////////////////////////////

        STAGE_T stage;
        std::ranges::transform(currentLogicGates.begin(),
                               currentLogicGates.end(),
                               std::back_inserter(stage.logic_gates),
                               [](LOGIC_GATE_T* logicGate)
                               {
                                   return *logicGate;
                               });
        stages.push_back(stage);

        ///////////////////////////////////////////////////////////////
        /// In order to get the logic gates left, we need to remove ///
        /// the current one we pushed earlier                       ///
        ///////////////////////////////////////////////////////////////
        logicGatesLeft.erase(
          std::remove_if(logicGatesLeft.begin(),
                         logicGatesLeft.end(),
                         [&currentLogicGates](LOGIC_GATE_T& logicGate)
                         {
                             return std::ranges::find(currentLogicGates,
                                                      &logicGate)
                                    != currentLogicGates.end();
                         }),
          logicGatesLeft.end());

        currentLogicGates.clear();
        currentOutputPorts.clear();
    }

    return stages;
}

#endif
