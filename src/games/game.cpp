#include "game.h"

#include "hl2_source_unpack.h"

#include <string>
#include <optional>
#include "../mem.h"
#include "../platform.h"

std::optional<std::filesystem::path> get_mod_dir(const char* target_mod) {
	auto mod_dir = std::string();
	auto module = mem::ModuleHandle::find(target_mod);
	if (module) {
		// TODO: platform-specific logic
		return (*module).path.parent_path().parent_path();
	}

	return std::nullopt;
}

Game* Game::identify() {
	auto mod_dir = get_mod_dir(MODULEFILE("engine"));
	if (!mod_dir) {
		return nullptr; // oops
	}
	auto modname = mod_dir->filename().string();

	if (modname == "source_unpack") {
		return new HL2SourceUnpack();
	}

	return nullptr; // oops
}
