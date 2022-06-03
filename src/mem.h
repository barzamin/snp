#pragma once

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#endif

#include <string>
#include <optional>

namespace mem {
	class ModuleHandle {
	public:
		std::string name;
		uintptr_t base;
		size_t size;
	private:
#ifdef _WIN32
		HMODULE handle;
		ModuleHandle(std::string name, uintptr_t base, size_t size, HMODULE handle) : name{ name }, base{ base }, size{ size }, handle{ handle } {}
#endif

	public:
		static std::optional<mem::ModuleHandle> find(const std::string& modname);

		template<typename T = void*>
		std::optional<T> symbol(const std::string& name) {
			std::optional<T> sym;
#ifdef _WIN32
			void* raw = GetProcAddress(this->handle, name.c_str());
#endif
			if (raw != nullptr) {
				sym = (T)raw;
			}

			return sym;
		}

		~ModuleHandle() {
			// todo dlclose
		}
	};
};