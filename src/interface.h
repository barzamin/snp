#pragma once

#include "mem.h"
#include "sdk.h"

#include <optional>

class Interface {
private:
	void* baseptr;
	uintptr_t* vtable;
	size_t vtable_len;
public:
	static auto from_mod(const char* mod_filename, const char* iface_sym) -> std::optional<Interface> {
		auto maybe_ptr = ptr_of(mod_filename, iface_sym);
		if (!maybe_ptr) {
			return std::nullopt;
		}

		return Interface(*maybe_ptr);
	}

	Interface(const Interface&) = delete;
	Interface(Interface&&) = default;

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

	static std::optional<void*> ptr_of(const char* mod_filename, const char* iface_sym) {
		auto handle = mem::ModuleHandle::find(mod_filename);

		if (!handle) {
			// warn
			return std::nullopt;
		}

		return ptr_of(*handle, iface_sym);
	}
	static std::optional<void*> ptr_of(mem::ModuleHandle& handle, const char* iface_sym) {
		auto createinterface = handle.symbol<CreateInterfaceFn>("CreateInterface");
		if (!createinterface) {
			// warn
			return std::nullopt;
		}

		int ret = 0;
		void* fn = (*createinterface)(iface_sym, &ret);

		if (ret) {
			// opes
			return std::nullopt;
		}

		return fn;
	}
};
