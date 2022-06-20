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

Tier1* Tier1::init(Game& game) {
	auto tier1 = new Tier1();

	auto mod_handle = mem::ModuleHandle::find(MODULEFILE(TIER1_NAME));
	if (!mod_handle) {
		spdlog::error("can't find Tier1 module ({})", MODULEFILE(TIER1_NAME));
		return nullptr;
	}
	tier1->g_pCVar = std::make_unique<Interface>(Interface::from_mod(*mod_handle, "VEngineCvar004").value());

	auto offsets = game.offsets();
	tier1->RegisterConCommand_ptr = tier1->g_pCVar->virt<_RegisterConCommand>(offsets->ICVar_RegisterConCommand);
	tier1->UnregisterConCommand_ptr = tier1->g_pCVar->virt<_UnregisterConCommand>(offsets->ICVar_UnregisterConCommand);
	tier1->FindCommandBase_ptr = tier1->g_pCVar->virt<_FindCommandBase>(offsets->ICVar_FindCommandBase);

	// snag the vtable from a command in the game
	auto listdemo = static_cast<ConCommand*>(tier1->FindCommandBase("listdemo"));
	if (listdemo) {
		tier1->vt_ConCommand = *(void**)listdemo;
	}

	return tier1;
}

void Tier1::shutdown()
{
}
