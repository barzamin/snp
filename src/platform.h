#pragma once

#define MODULEFILE(name) (name DYLIB_EXTENSION)

#ifdef _WIN32
#define DYLIB_EXTENSION ".dll"
#define DYLIB_EXPORT extern "C" __declspec(dllexport)

#define CONCOLORMSG_SYMBOL "?ConColorMsg@@YAXABVColor@@PBDZZ"
#define DEVMSG_SYMBOL "?DevMsg@@YAXPBDZZ"
#define DEVWARNINGMSG_SYMBOL "?DevWarning@@YAXPBDZZ"

#else // assume Linux
#define DYLIB_EXTENSION ".so"
#define DYLIB_EXPORT extern "C" __attribute__((visibility("default")))
#endif
