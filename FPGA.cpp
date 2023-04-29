#include "FPGA.h"

FPGA::AllPinsAndPortsEncoded::return_t FPGA::AllPinsAndPortsEncoded::
  Generate()
{
    std::vector<Pin*> inputPins(number_of_input_pins);
    std::vector<Pin*> outputPins(number_of_output_pins);
    std::vector<Port*> othersPorts(number_of_others_ports);

    static const auto GenerateFunction = [&]()
    {
        return new Port { 0 };
    };

    std::generate(inputPins.begin(), inputPins.end(), GenerateFunction);
    std::generate(outputPins.begin(), outputPins.end(), GenerateFunction);
    std::generate(othersPorts.begin(),
                  othersPorts.end(),
                  GenerateFunction);

    return { inputPins, outputPins, othersPorts };
}

FPGA::FPGA(Header* header)
{
    auto result  = header->all_pins_and_ports_record.Generate();
    _input_pins  = std::get<0>(result);
    _output_pins = std::get<1>(result);

    _ports.insert(_ports.end(), _input_pins.begin(), _input_pins.end());
    _ports.insert(_ports.end(), _output_pins.begin(), _output_pins.end());

    _ports.insert(_ports.end(),
                  std::get<2>(result).begin(),
                  std::get<2>(result).end());

    std::vector<LogicGate*> logicGates;

    const auto firstLogicGateEncoded = reinterpret_cast<
      LogicGate::Encoded*>(reinterpret_cast<std::uintptr_t>(this)
                           + header->offset_to_first_logic_gate);

    std::for_each_n(
      firstLogicGateEncoded,
      header->number_of_logic_gates,
      [&](LogicGate::Encoded& logicGateEncoded)
      {
          const auto logicGate = logicGateEncoded.ToLogicGate(_ports);

          logicGates.push_back(new LogicGate { logicGate });
      });

    ////////////////////////////////////////////
    /// Check dependecy and construct stages ///
    ////////////////////////////////////////////
}

FPGA::~FPGA()
{
}
