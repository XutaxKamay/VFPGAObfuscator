#include "Timer.h"
#include "VFPGA.h"

using namespace VFPGAObfuscatorLibrary;
using namespace VFPGAObfuscatorSimulator;

static consteval EncodedIndex operator""_p(unsigned long long i)
{
    return i;
}

int main()
{
    std::cout << "Serializing test ...\n";

    std::vector<LogicGate::Serializer> logicGatesSerializer;

    logicGatesSerializer.push_back({
      { 0_p },
      { 1_p },
      { { 0_vfpga_bit }, { 1_vfpga_bit } },
      { { 1_vfpga_bit }, { 0_vfpga_bit } }
    });

    for (std::size_t i = 0; i < 30; i++)
    {
        for (EncodedIndex j = 1; j < 10001; j++)
        {
            logicGatesSerializer.push_back(
              { { j }, { j }, { { j } }, { { j } } });
        }
    }

    VFPGA::Serializer vfpgaSerializer { 1000000, logicGatesSerializer };

    VFPGA::Deserializer vfpgaDeserializer;
    auto vfpga = vfpgaDeserializer.Deserialize(
      vfpgaSerializer.Serialize<true>());

    vfpga.GetPort(0)->SetLow();
    std::cout << "VFPGA test ... \n";
    std::cout << "Number of stages: " << vfpga.stages.size() << '\n';

    for (std::size_t i = 0; i < vfpga.stages.size(); i++)
    {
        std::cout << "Number of logic gates in stage " << i << ":"
                  << vfpga.stages[i].logic_gates.size() << '\n';
    }

    Timer timerVFPGA;
    Timer timerPrint;
    timerPrint.Start();

    std::int_fast64_t averageTime      = 0;
    std::int_fast64_t countAverageTime = 0;
    vfpga.PrepareStages();

    while (true)
    {
        timerVFPGA.Start();
        vfpga.Simulate();
        timerVFPGA.End();

        averageTime += timerVFPGA.Difference();
        countAverageTime++;

        if (timerPrint.Seconds() >= 10)
        {
            std::cout
              << "Processed " << vfpga.LogicGates().size()
              << " logic gates in "
              << static_cast<double>(averageTime)
                   / static_cast<double>(countAverageTime) / 10000000.0
              << " milliseconds of average time for ten seconds\n";
            countAverageTime = 0;
            averageTime      = 0;

            std::cout << "port 1 should have state 1: "
                      << static_cast<int>(vfpga.GetPort(1)->state)
                      << '\n';

            timerPrint.Start();
        }
        else
        {
            timerPrint.End();
        }
    }
}
