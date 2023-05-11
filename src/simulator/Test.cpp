#include "FPGA.h"
#include "Timer.h"

using namespace FPGASimulator;

static constexpr EncodedIndex operator""_p(unsigned long long index)
{
    return index;
}

static constexpr Bit operator""_b(unsigned long long index)
{
    return static_cast<std::uint8_t>(index);
}

int main()
{
    std::cout << "Serializing test ...\n";

    std::vector<LogicGate::Serializer> logicGatesSerializer;

    logicGatesSerializer.push_back({
      { 0_p },
      { 1_p },
      { { 0_b }, { 1_b } },
      { { 1_b }, { 0_b } }
    });

    for (std::size_t index = 2; index < 600002; index += 2)
    {
        logicGatesSerializer.push_back({
          { index },
          { index + 1 },
          { { 0_b }, { 1_b } },
          { { 1_b }, { 0_b } }
        });

        logicGatesSerializer.push_back({
          { index + 1 },
          { index },
          { { 0_b }, { 1_b } },
          { { 1_b }, { 0_b } }
        });
    }

    logicGatesSerializer.push_back(
      { { 1_p }, { 1_p }, { { 1_p } }, { { 1_p } } });

    logicGatesSerializer.push_back(
      { { 1_p }, { 1_p }, { { 1_p } }, { { 1_p } } });

    FPGA::Serializer fpgaSerializer { 600002, logicGatesSerializer };

    FPGA::Deserializer fpgaDeserializer;
    auto fpga = fpgaDeserializer.Deserialize(
      fpgaSerializer.Serialize<true>());

    fpga.GetPort(0)->SetLow();
    std::cout << "FPGA test ... \n";
    std::cout << "Number of stages: " << fpga.stages.size() << '\n';

    Timer timerFPGA;
    Timer timerPrint;
    timerPrint.Start();

    std::int64_t averageTime      = 0;
    std::int64_t countAverageTime = 0;
    fpga.PrepareStages();

    while (true)
    {
        timerFPGA.Start();
        fpga.Simulate();
        timerFPGA.End();

        averageTime += timerFPGA.Difference();
        countAverageTime++;

        if (timerPrint.Seconds() >= 10)
        {
            std::cout
              << "Processed " << fpga.LogicGates().size()
              << " logic gates in "
              << static_cast<double>(averageTime)
                   / static_cast<double>(countAverageTime) / 10000000.0
              << " milliseconds of average time for ten seconds\n";
            countAverageTime = 0;
            averageTime      = 0;

            std::cout << "port 1 should have state 1: "
                      << fpga.GetPort(1)->state << '\n';

            timerPrint.Start();
        }
        else
        {
            timerPrint.End();
        }
    }
}
