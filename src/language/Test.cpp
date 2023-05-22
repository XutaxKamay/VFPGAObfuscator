#include "NOTLogicGate.h"
#include "XORLogicGate.h"

#include "simulator/VFPGA.h"

using namespace VFPGAObfuscatorLibrary;

int main()
{
    enum Port : EncodedIndex
    {
        A,
        B,
        C,
        D,
        ALL
    };

    VFPGAObfuscatorLanguage::XORLogicGate xorLogicGate {
        { Port::A, Port::B, Port::C },
        { Port::D }
    };

    VFPGAObfuscatorSimulator::VFPGA::Serializer vfpgaSerializer {
        Port::ALL,
        {xorLogicGate,
               VFPGAObfuscatorLanguage::NOTLogicGate(Port::D, { Port::D })}
    };

    VFPGAObfuscatorSimulator::VFPGA::Deserializer vpfgaDeserializer;

    auto vfpga = vpfgaDeserializer.Deserialize(
      vfpgaSerializer.Serialize<true>());

    vfpga.PrepareStages();

    vfpga.GetPort(Port::A)->SetLow();
    vfpga.GetPort(Port::B)->SetLow();
    vfpga.GetPort(Port::C)->SetHigh();

    std::cout << "Stages: " << vfpga.stages.size() << '\n';

    vfpga.Simulate();

    std::cout << static_cast<int>(vfpga.GetPort(Port::D)->state) << '\n';

    return 0;
}
