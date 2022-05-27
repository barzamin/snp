#pragma once

#ifdef _WIN32
#define MODULE_EXTENSION ".dll"
#define DYLIB_EXPORT extern "C" __declspec(dllexport)
#else // assume Linux
#define MODULE_EXTENSION ".so"
#define DYLIB_EXPORT extern "C" __attribute__((visibility("default")))
#endif
