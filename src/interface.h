#pragma once

#include "mem.h"
#include "sdk.h"

#include <spdlog/spdlog.h>
#include <optional>

class Interface {
private:
	void* baseptr;
	uintptr_t* vtable;
	size_t vtable_len;
public:
	static auto from_mod(mem::ModuleHandle& mod, const char* iface_sym) -> std::optional<Interface> {
		auto maybe_ptr = ptr_of(mod, iface_sym);
		if (!maybe_ptr) {
			return std::nullopt;
		}

		return Interface(*maybe_ptr);
	}

	static auto from_mod(const char* mod_filename, const char* iface_sym) -> std::optional<Interface> {
		auto handle = mem::ModuleHandle::find(mod_filename);

		if (!handle) {
			spdlog::error("trying to locate an Interface's module ({}) but we can't get a handle");
			return std::nullopt;
		}

		auto maybe_ptr = ptr_of(*handle, iface_sym);
		if (!maybe_ptr) {
			return std::nullopt;
		}

		return Interface(*maybe_ptr);
	}

	Interface(const Interface&) = delete;
	Interface(Interface&&) = default;

	template<typename T>
	T virt(size_t idx) {
		return reinterpret_cast<T>(this->vtable[idx]);
	}

	void* point() {
		return reinterpret_cast<void*>(this->baseptr);
	}

private:
	Interface(void* baseptr) {
		this->baseptr = baseptr;
		this->vtable = *(uintptr_t**)baseptr;
		this->vtable_len = 0;
		while (this->vtable[this->vtable_len]) {
			this->vtable_len++;
		}
	}

	Interface() : baseptr(nullptr), vtable(nullptr), vtable_len(0) {}

	static std::optional<void*> ptr_of(mem::ModuleHandle& handle, const char* iface_sym) {
		auto createinterface = handle.symbol<CreateInterfaceFn>("CreateInterface");
		if (!createinterface) {
			spdlog::error("can't find CreateInterface in given module");
			return std::nullopt;
		}

		int ret = 0;
		void* fn = (*createinterface)(iface_sym, &ret);

		if (ret) {
			spdlog::error("CreateInterface() failed with errno {}", ret);
			return std::nullopt;
		}

		return fn;
	}
};
