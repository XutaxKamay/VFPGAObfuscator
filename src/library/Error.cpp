#include "Error.h"

using namespace VFPGAObfuscatorLibrary;

void VFPGAObfuscatorLibrary::Error::ExitWithMsg(Msg msg)
{
    std::exit(static_cast<int>(msg));
}
