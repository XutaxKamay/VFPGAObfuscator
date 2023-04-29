#include "FPGA.h"

FPGA::AllPinsAndPortsRecord::return_t FPGA::AllPinsAndPortsRecord::Generate()
{
    std::vector<Pin*> inputPins(number_of_input_pins);
    std::vector<Pin*> outputPins(number_of_output_pins);
    std::vector<Port*> ports(number_of_ports);

    static const auto generateFunc = [&]()
    {
        return new Port { 0 };
    };

    std::generate(inputPins.begin(), inputPins.end(), generateFunc);
    std::generate(outputPins.begin(), outputPins.end(), generateFunc);
    std::generate(ports.begin(), ports.end(), generateFunc);

    return { inputPins, outputPins, ports };
}

std::vector<FPGA::index_t> FPGA::PortsIndexesRecord::Read()
{
    std::vector<index_t> indexes(number_of_ports_indexes);

    std::for_each_n(&first_port_index,
                    number_of_ports_indexes,
                    [&](const auto& index)
                    {
                        indexes.push_back(index);
                    });

    return indexes;
}

std::vector<Port*> FPGA::LogicGateRecord::GetInputPorts(
  const std::vector<Port*>& allPorts)
{
    std::vector<Port*> inputPorts;

    std::for_each_n(
      reinterpret_cast<index_t*>(reinterpret_cast<std::uintptr_t>(this)
                                 + offset_to_first_input_port),
      number_of_input_ports,
      [&](const auto& index)
      {
          inputPorts.push_back(allPorts.at(index));
      });

    return inputPorts;
}

std::vector<Port*> FPGA::LogicGateRecord::GetOutputPorts(
  const std::vector<Port*>& allPorts)
{
    std::vector<Port*> outputPorts;

    std::for_each_n(
      reinterpret_cast<index_t*>(reinterpret_cast<std::uintptr_t>(this)
                                 + offset_to_first_output_port),
      number_of_output_ports,
      [&](const auto& index)
      {
          outputPorts.push_back(allPorts.at(index));
      });

    return outputPorts;
}

LogicGate::Decoder FPGA::LogicGateRecord::DecodeLogicFunction(
  const std::vector<Port*>& allPorts)
{
    LogicGate::Decoder decoder;

    //////////////////////////////
    /// Decode the truth table ///
    //////////////////////////////

    auto truthTableElements = reinterpret_cast<TruthTableElement*>(
      reinterpret_cast<std::uintptr_t>(this) + offset_to_truth_table);

    for (std::size_t i = 0; i < number_of_lines_in_truth_table; i++)
    {
        std::vector<std::variant<Port*, Bit>> line;

        static const auto decodedElements =
          [&](TruthTableElement& truthTableElement)
        {
            switch (truthTableElement.type)
            {
                case TruthTableElement::type_t::BOOL:
                {
                    const auto boolElement = reinterpret_cast<
                      BoolInTruthTable*>(&truthTableElement);
                    line.push_back(boolElement->value);
                    break;
                }

                case TruthTableElement::type_t::PORT:
                {
                    const auto portElement = reinterpret_cast<
                      PortInTruthTable*>(&truthTableElement);
                    line.push_back(allPorts.at(portElement->port_index));
                    break;
                }
            }
        };

        std::for_each_n(truthTableElements,
                        number_of_input_ports,
                        decodedElements);

        decoder.input_truth_table.push_back(line);
        line.clear();

        std::for_each_n(truthTableElements + number_of_input_ports,
                        number_of_output_ports,
                        decodedElements);

        decoder.output_truth_table.push_back(line);
    }

    return decoder;
}

LogicGate FPGA::LogicGateRecord::ToLogicGate(
  const std::vector<Port*>& allPorts)
{
    const auto inputPorts  = GetInputPorts(allPorts);
    const auto outputPorts = GetOutputPorts(allPorts);
    const auto decoder     = DecodeLogicFunction(allPorts);

    return { inputPorts, outputPorts, decoder };
}
