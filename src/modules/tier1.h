#pragma once

#include "module.h"
#include "../interface.h"
#include "../sdk.h"

#include <string>
#include <memory>
#include <optional>

class Game;

class Tier1 {
public:
	Tier1() = delete;
	Tier1(Game& game);
	Tier1(const Tier1&) = delete;
	Tier1(Tier1&&) = default;
	void shutdown();

	// checked proxies so we dont call nullptr (and we can hide g_pCVar).
	// todo: make assert vanish in release, and maybe make a macro for this.
	void RegisterConCommand(ConCommandBase* pCommandBase) {
		assert(this->RegisterConCommand_ptr != nullptr);
		this->RegisterConCommand_ptr(g_pCVar->point(), pCommandBase);
	}
	void UnregisterConCommand(ConCommandBase* pCommandBase) {
		assert(this->UnregisterConCommand_ptr != nullptr);
		this->UnregisterConCommand_ptr(g_pCVar->point(), pCommandBase);
	}
	ConCommandBase* FindCommandBase(const char* name) {
		assert(this->FindCommandBase_ptr != nullptr);
		return this->FindCommandBase_ptr(g_pCVar->point(), name);
	}

	// vtable theft time
	void* vt_ConCommand = nullptr;
private:

	_RegisterConCommand RegisterConCommand_ptr = nullptr;
	_UnregisterConCommand UnregisterConCommand_ptr = nullptr;
	_FindCommandBase FindCommandBase_ptr = nullptr;

	std::unique_ptr<Interface> g_pCVar;
};
