#include "FPGA.h"
#include "Timer.h"

using namespace FPGASimulator;

static constexpr EncodedIndex operator""_p(unsigned long long i)
{
    return i;
}

static constexpr Bit operator""_b(unsigned long long i)
{
    return static_cast<std::uint8_t>(i);
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

    for (std::size_t i = 0; i < 30; i++)
    {
        for (EncodedIndex j = 1; j < 10001; j++)
        {
            logicGatesSerializer.push_back(
              { { j }, { j }, { { j } }, { { j } } });
        }
    }

    FPGA::Serializer fpgaSerializer { 1000000, logicGatesSerializer };

    FPGA::Deserializer fpgaDeserializer;
    auto fpga = fpgaDeserializer.Deserialize(
      fpgaSerializer.Serialize<true>());

    fpga.GetPort(0)->SetLow();
    std::cout << "FPGA test ... \n";
    std::cout << "Number of stages: " << fpga.stages.size() << '\n';

    for (std::size_t i = 0; i < fpga.stages.size(); i++)
    {
        std::cout << "Number of logic gates in stage " << i << ":"
                  << fpga.stages[i].logic_gates.size() << '\n';
    }

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
                      << static_cast<int>(fpga.GetPort(1)->state) << '\n';

            timerPrint.Start();
        }
        else
        {
            timerPrint.End();
        }
    }
}
