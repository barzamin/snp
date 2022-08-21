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

Tier1::Tier1(Game& game) {
	auto mod = mem::ModuleHandle::find(MODULEFILE(TIER1_NAME));
	if (!mod) {
		spdlog::error("can't find Tier1 module ({})", MODULEFILE(TIER1_NAME));
		throw ModuleLoadError();
	}
	this->g_pCVar = std::make_unique<Interface>(Interface::from_mod(*mod, "VEngineCvar004").value());

	auto offsets = game.offsets();
	this->RegisterConCommand_ptr = this->g_pCVar->virt<_RegisterConCommand>(offsets->ICVar_RegisterConCommand);
	this->UnregisterConCommand_ptr = this->g_pCVar->virt<_UnregisterConCommand>(offsets->ICVar_UnregisterConCommand);
	this->FindCommandBase_ptr = this->g_pCVar->virt<_FindCommandBase>(offsets->ICVar_FindCommandBase);

	// snag the vtable from a command in the game
	auto listdemo = static_cast<ConCommand*>(this->FindCommandBase("listdemo"));
	if (listdemo) {
		this->vt_ConCommand = *(void**)listdemo;
	}
}

void Tier1::shutdown()
{
}
