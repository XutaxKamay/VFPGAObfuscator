#include "Error.h"

using namespace VFPGAObfuscatorLibrary;

void Error::ExitWithMsg(Msg msg)
{
    std::exit(static_cast<int>(msg));
}
