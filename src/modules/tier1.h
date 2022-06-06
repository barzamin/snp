#pragma once

#include "module.h"
#include "../interface.h"
#include "../sdk.h"

#include <string>
#include <memory>
#include <optional>

class Tier1 {
public:
	static std::optional<Tier1> init();
	void shutdown();

	Tier1() {}
	Tier1(const Tier1&) = delete;
	Tier1(Tier1&&) = default;

private:
	_RegisterConCommand RegisterConCommand = nullptr;
	_UnregisterConCommand UnregisterConCommand = nullptr;

	std::unique_ptr<Interface> g_pCVar;
};
