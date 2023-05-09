#include "FPGA.h"
#include "Timer.h"

using namespace FPGASimulator;

static constexpr EncodedIndex operator""_p(unsigned long long index)
{
    return index;
}

static constexpr Bit operator""_b(unsigned long long index)
{
    return index;
}

int main()
{
    std::cout << "Serializing test ...\n";

    LogicGate::Serializer logicGateInverterSerializer;
    logicGateInverterSerializer.input_ports       = { 0_p };
    logicGateInverterSerializer.output_ports      = { 0_p };
    logicGateInverterSerializer.input_truth_table = {
        { 0_b },
        { 1_b },
    };

    logicGateInverterSerializer.output_truth_table = {
        {1_b, 0_b}
    };

    FPGA::Serializer fpgaSerializer { 1,
                                      { logicGateInverterSerializer } };

    FPGA::Deserializer fpgaDeserializer;
    auto fpga = fpgaDeserializer.Deserialize(fpgaSerializer.Serialize());

    std::cout << "FPGA test ... \n";

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
