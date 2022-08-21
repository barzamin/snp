#include "console.h"

#include "../platform.h"
#include "tier1.h"


#ifdef _WIN32
#define TIER0_NAME "tier0"
#else
#define TIER0_NAME "libtier0"
#endif


Console::Console(Tier1& tier1) {
	auto mod = mem::ModuleHandle::find(MODULEFILE(TIER0_NAME));
	if (!mod) {
		spdlog::error("can't find Tier0 module ({})", MODULEFILE(TIER0_NAME));
		throw ModuleLoadError();
	}

	auto ConColorMsg_ptr = *mod->symbol<_ConColorMsg>(CONCOLORMSG_SYMBOL);
	if (!ConColorMsg_ptr) {
		spdlog::error("cant find ConColorMsg() in tier1 module");
		throw new ModuleLoadError();
	}
	this->ConColorMsg_ptr = *ConColorMsg_ptr;
}

void Console::shutdown()
{
}
