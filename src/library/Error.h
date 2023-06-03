#ifndef VFPGA_OBFUSCATOR_LIBRARY_ERROR_H
#define VFPGA_OBFUSCATOR_LIBRARY_ERROR_H

#include "StandardHeaders.h"

namespace VFPGAObfuscatorLibrary
{
    class Error
    {
      public:
        enum struct Msg : int
        {
            UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE,
            VFPGA_LOGIC_GATE_DESERIALIZED_INPUTS_IN_TRUTH_TABLE_NOT_CORRECT,
            VFPGA_LOGIC_GATE_DESERIALIZED_OUTPUTS_IN_TRUTH_TABLE_NOT_CORRECT,
            VFPGA_LOGIC_GATE_DESERIALIZED_TRUTH_TABLE_NOT_CORRECT,
            LOGIC_GATE_DESERIALIZER_READ_FAILED,
            LOGIC_GATE_NOT_ENOUGH_INPUT_PORTS,
            DESERIALIZER_READ_FAILED
        };

        [[noreturn]] static void ExitWithMsg(Msg msg);
    };
}

#endif
