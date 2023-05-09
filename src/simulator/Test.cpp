#include "Deserializer.h"
#include "FPGA.h"
#include "Serializer.h"
#include "Timer.h"

using namespace FPGASimulator;

int main()
{
    std::cout << "Serializing test ...\n";

    Serializer serializer;
    serializer.AddVar<std::uint16_t>(1337);
    serializer.AddVar<std::intmax_t>(1337);
    serializer.AddVar(std::vector<std::byte> { std::byte { 1 },
                                               std::byte { 3 },
                                               std::byte { 3 },
                                               std::byte { 7 } });

    Deserializer deserializer { serializer.data };

    const auto var_1337   = deserializer.ReadVar<std::uint16_t>();
    const auto var_1337_2 = deserializer.ReadVar<std::intmax_t>();
    const auto test_2 = deserializer.ReadVar<std::vector<std::byte>>();

    std::cout << var_1337 << ' ' << var_1337_2 << ' ';

    std::ranges::for_each(test_2,
                          [](const std::byte& byte)
                          {
                              std::cout << static_cast<const int>(byte);
                          });
    std::cout << '\n';

    std::cout << "FPGA test ... \n";

    FPGA fpga(100000);

    LogicGate logicGate {
        { fpga.GetPort(0), fpga.GetPort(1) },
        { fpga.GetPort(2) },
        { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0 }, { 0 }, { 0 }, { 1 } }
    };

    fpga.InsertLogicGate(logicGate);

    for (auto inputPort : logicGate.input_ports)
    {
        inputPort->SetHigh();
    }

    fpga.InsertLogicGate({
      { fpga.GetPort(2) },
      { fpga.GetPort(2) },
      { { 0 }, { 1 } },
      { { 1 }, { 0 } }
    });

    for (std::size_t i = 3; i < 100000 - 1;)
    {
        fpga.InsertLogicGate({
          { fpga.GetPort(i) },
          { fpga.GetPort(i + 1) },
          { { 0 }, { 1 } },
          { { 1 }, { 0 } }
        });

        i += 2;
    }

    fpga.PrepareStages();

    Timer timerFPGA;
    Timer timerPrint;
    timerPrint.Start();

    std::int64_t averageTime      = 0;
    std::int64_t countAverageTime = 0;

    while (true)
    {
        timerFPGA.Start();
        fpga.Simulate();
        timerFPGA.End();

        averageTime += timerFPGA.Difference();
        countAverageTime++;

        if (timerPrint.Seconds() >= 1)
        {
            std::cout << averageTime / countAverageTime
                      << " nanoseconds of average time in one second\n";
            countAverageTime = 0;
            averageTime      = 0;

            timerPrint.Start();
        }
        else
        {
            timerPrint.End();
        }
    }
}
