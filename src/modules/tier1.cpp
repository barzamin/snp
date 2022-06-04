#include "tier1.h"
#include "../platform.h"

#ifdef _WIN32
#define TIER1_NAME "vstdlib"
#else
#define TIER1_NAME "libvstdlib"
#endif

bool Tier1::init() {
	this->g_pCVar = std::make_unique<Interface>(Interface::from_mod(MODULEFILE(TIER1_NAME), "VEngineCvar004").value());



	return false;
}

void Tier1::shutdown()
{
}
