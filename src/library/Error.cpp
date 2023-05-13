#include "Error.h"

using namespace FPGAObfuscatorLibrary;

void Error::ExitWithMsg(Msg msg)
{
    std::exit(static_cast<int>(msg));
}
