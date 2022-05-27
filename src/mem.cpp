#include "mem.h"

std::optional<mem::ModuleHandle> mem::ModuleHandle::find(const std::string& modname) {
#ifdef _WIN32
	HANDLE hProcess = GetCurrentProcess();
	HMODULE hModules[1024]; // TODO: resize dynamically based on lpcbNeeded
	DWORD cbNeeded;
	if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
		for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); i++) {
			char cur_mod_path_arr[MAX_PATH];
			if (!GetModuleFileNameA(hModules[i], cur_mod_path_arr, sizeof(cur_mod_path_arr))) {
				continue;
			}

			MODULEINFO modinfo;
			if (!GetModuleInformation(hProcess, hModules[i], &modinfo, sizeof(modinfo))) {
				continue;
			}

			auto cur_mod_path = std::string(cur_mod_path_arr);
			auto idx = cur_mod_path.find_last_of("\\");
			auto cur_mod_name = cur_mod_path.substr(idx + 1, cur_mod_path.length() - idx); // slice out everything after last path sep

			if (cur_mod_name == modname) {
				return mem::ModuleHandle(cur_mod_name, (uintptr_t)modinfo.lpBaseOfDll, (size_t)modinfo.SizeOfImage, hModules[i]);
			}
		}
	}

	return std::nullopt;
#endif
}
