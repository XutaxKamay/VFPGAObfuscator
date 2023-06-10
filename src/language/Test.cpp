#include "ADDLogicGate.h"
#include "ANDLogicGate.h"
#include "BUFLogicGate.h"
#include "ORLogicGate.h"
#include "XORLogicGate.h"

#include "library/CExpressionUtils.h"
#include "simulator/VFPGA.h"

using namespace VFPGAObfuscatorLibrary;
using namespace VFPGAObfuscatorLanguage;

static void VFPGAExampleInHeader()
{
    //////////////////////////////////////////////////////
    /// Let's copy the scheme inside the VFPGA example ///
    //////////////////////////////////////////////////////
    enum Port : VFPGAObfuscatorLanguage::LogicGate::Port
    {
        A_IN,
        B_IN,
        C_IN,
        D_IN,
        E_IN,
        F_IN,
        A_OUT,
        B_OUT,
        C_OUT,
        D_OUT,
        E_OUT,
        F_OUT,
        TEMP_STAGE1_1,
        TEMP_STAGE1_2,
        TEMP_STAGE1_3,
        TEMP_STAGE2_1,
        TEMP_STAGE2_2,
        ALL
    };

    struct GenerateVFPGA
    {
        constexpr auto operator()()
        {
            const std::vector<LogicGate::Serializer> logicGates = {
                ANDLogicGate(
                  {
                    Port::A_IN,
                    Port::B_IN,
                  },
                  { Port::TEMP_STAGE1_1 }),

                ORLogicGate(
                  {
                    Port::C_IN,
                    Port::D_IN,
                  },
                  { Port::TEMP_STAGE1_2 }),

                XORLogicGate(
                  {
                    Port::E_IN,
                    Port::F_IN,
                  },
                  { Port::TEMP_STAGE1_3 }),

                ORLogicGate(
                  {
                    Port::TEMP_STAGE1_1,
                    Port::TEMP_STAGE1_2,
                  },
                  { Port::TEMP_STAGE2_1 }),

                ANDLogicGate(
                  {
                    Port::TEMP_STAGE1_2,
                    Port::TEMP_STAGE1_3,
                  },
                  { Port::TEMP_STAGE2_2 }),

                BUFLogicGate(Port::TEMP_STAGE2_1,
                             { Port::A_OUT, Port::B_OUT, Port::C_OUT }),

                BUFLogicGate(Port::TEMP_STAGE2_2,
                             { Port::D_OUT, Port::E_OUT, Port::F_OUT })
            };

            VFPGAObfuscatorSimulator::VFPGA::Serializer vfpgaSerializer {
                Port::ALL,
                logicGates,
                {{ Port::A_IN, 1_vfpga_obf_lib_bit },
                  { Port::B_IN, 0_vfpga_obf_lib_bit },
                  { Port::C_IN, 0_vfpga_obf_lib_bit },
                  { Port::D_IN, 1_vfpga_obf_lib_bit },
                  { Port::E_IN, 1_vfpga_obf_lib_bit },
                  { Port::F_IN, 1_vfpga_obf_lib_bit }}
            };

            return vfpgaSerializer.Serialize<true>();
        }
    };

    static constexpr auto vfpgaSerialized = CExpressionUtils::
      VectorToArray<GenerateVFPGA>();

    VFPGAObfuscatorSimulator::VFPGA::Deserializer vfpgaDeserializer;

    auto vfpga = vfpgaDeserializer.Deserialize({ vfpgaSerialized.begin(),
                                                 vfpgaSerialized.end() });

    vfpga.PrepareStages();

    std::cout << "Stages: " << vfpga.stages.size() << '\n';

    vfpga.Simulate();

    std::cout << "Result:\n";
    std::cout << static_cast<int>(vfpga.GetPort(Port::A_OUT)->state)
              << static_cast<int>(vfpga.GetPort(Port::B_OUT)->state)
              << static_cast<int>(vfpga.GetPort(Port::C_OUT)->state)
              << static_cast<int>(vfpga.GetPort(Port::D_OUT)->state)
              << static_cast<int>(vfpga.GetPort(Port::E_OUT)->state)
              << static_cast<int>(vfpga.GetPort(Port::F_OUT)->state)
              << '\n';
}

static void VFPGAAddIntegerTests()
{
    static constexpr auto integerBitSize = 2;

    enum Port : VFPGAObfuscatorLanguage::LogicGate::Port
    {
        INTEGER_1      = 0,
        INTEGER_2      = integerBitSize,
        INTEGER_RESULT = INTEGER_2 + integerBitSize,
        CARRY_BIT      = INTEGER_RESULT + integerBitSize,
        ALL
    };

    struct GenerateVFPGA
    {
        constexpr auto operator()()
        {
            VFPGAObfuscatorSimulator::VFPGA::Serializer vfpgaSerializer {};

            return vfpgaSerializer.Serialize<true>();
        }
    };

    static constexpr auto vfpgaSerialized = CExpressionUtils::
      VectorToArray<GenerateVFPGA>();

    VFPGAObfuscatorSimulator::VFPGA::Deserializer vfpgaDeserializer;

    auto vfpga = vfpgaDeserializer.Deserialize({ vfpgaSerialized.begin(),
                                                 vfpgaSerialized.end() });

    vfpga.PrepareStages();

    std::cout << "Stages: " << vfpga.stages.size() << '\n';

    vfpga.Simulate();

    std::cout << "Result:\n";
}

int main()
{
    VFPGAExampleInHeader();
    VFPGAAddIntegerTests();

    return 0;
}
