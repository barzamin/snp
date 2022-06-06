#include "tier1.h"
#include "../platform.h"
#include "../games/game.h"
#include "../services.h"

#include <spdlog/spdlog.h>

#ifdef _WIN32
#define TIER1_NAME "vstdlib"
#else
#define TIER1_NAME "libvstdlib"
#endif

std::optional<Tier1> Tier1::init() {
	auto tier1 = Tier1();

	auto mod_handle = mem::ModuleHandle::find(MODULEFILE(TIER1_NAME));
	if (!mod_handle) {
		spdlog::error("can't find Tier1 module ({})", MODULEFILE(TIER1_NAME));
		return std::nullopt;
	}
	tier1.g_pCVar = std::make_unique<Interface>(Interface::from_mod(*mod_handle, "VEngineCvar004").value());

	auto offsets = Services::get<Game>()->offsets();
	tier1.RegisterConCommand = tier1.g_pCVar->virt<_RegisterConCommand>(offsets->ICVar_RegisterConCommand);
	tier1.UnregisterConCommand = tier1.g_pCVar->virt<_RegisterConCommand>(offsets->ICVar_UnregisterConCommand);

	return tier1;
}

void Tier1::shutdown()
{
}
