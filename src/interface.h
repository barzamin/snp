#pragma once

#include "mem.h"
#include "sdk.h"

#include <spdlog/spdlog.h>
#include <optional>
#include <vector>
#include <cstdint>

#define IFACE_VTABLE_BOUNDS_CHECKS

//! Abstraction layer over Source "interfaces", which are just (mostly-)abstract classes
//! exposed by the dlls in the engine as their main api surface. Thus, most of this class
//! just handles crawling through the vtable or editing it to hook functions.
class Interface {
private:
	//! base pointer of the original interface object
	void* baseptr;
	//! pointer to the vtable of the interface object
	uintptr_t* vtable_og;
	//! vtable's size, computed by crawling the vtable until we find a null
	size_t vtable_len;

	//! ptr to copied vtable (if requested; otherwise nullptr).
	//! DANGER: this points AT THE START OF THE TABLE ALLOCATION (the CompleteObjectLocator*),
	//! NOT THE START OF THE VTABLE; to point at the first virtual fn, you want (vtable_copy+1).
	std::unique_ptr<uintptr_t[]> vtable_copy;
public:
	//! create an Interface from a dynamic module (referenced by a mem::ModuleHandle)
	//! in the process's memory space, given the interface id symbol
	static auto from_mod(mem::ModuleHandle& mod, const char* iface_sym) -> std::optional<Interface> {
		auto maybe_ptr = ptr_of(mod, iface_sym);
		if (!maybe_ptr) {
			return std::nullopt;
		}

		return Interface(*maybe_ptr);
	}

	//! create an Interface from a dynamic module (looking it up by filename)
	//! in the process's memory space, given the interface factory's symbol name
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

	//! retrieve a pointer (casted to `T`) to a vtable method, indexed by `idx`. note that this retrieves
	//! a pointer to whatever mode the method is in - either the original vtable, the copied vtable, or
	//! the virtual method hook (which implies the entire vtable being copied).
	template<typename T>
	T virt(size_t idx) const {
		_check_idx(idx);

		if (this->vtable_copy != nullptr) {
			return reinterpret_cast<T>(this->vtable_copy[idx]);
		} else {
			return reinterpret_cast<T>(this->vtable_og[idx]);
		}
	}

	//! retrieve a pointer (casted to `T`) to a vtable method, indexed by `idx`. the method in the original vmt
	//! will always be returned, even if a copied vtable exists.
	template<typename T>
	T og_virt(size_t idx) const {
		_check_idx(idx);

		return reinterpret_cast<T>(this->vtable_og[idx]);
	}

	//! retrieve a void* pointer to the inner interface objec
	void* point() const {
		return reinterpret_cast<void*>(this->baseptr);
	}

	//! hook a virtal method, splicing in a pointer to a new implementation.
	//! returns the original method wrapped in a std::optional if we have a vtable copy set up,
	//! or std::nullopt if there's no copy configured.
	template<typename F = void*, typename T = void*>
	std::optional<T> hook(size_t idx, F detour) {
		_check_idx(idx);

		if (this->vtable_copy == nullptr) return std::nullopt;

		// +1 to skip the completeobjectlocator pointer
		this->vtable_copy[1 + idx] = detour;
		return this->og_virt<T>(idx);
	}

	//! enable hooked vmt; this effectively just swaps the vtable pointer in the underlying object
	//! with a pointer to our copy of the vtable.
	void enable_hooks() {
		*reinterpret_cast<uintptr_t**>(this->baseptr) = &this->vtable_copy[1];
	}

	void disable_hooks() {

	}

private:
	Interface(void* baseptr, bool copy_vtable = false) {
		this->baseptr = baseptr;
		this->vtable_og = *reinterpret_cast<uintptr_t**>(baseptr);
		this->vtable_len = 0;
		while (this->vtable_og[this->vtable_len]) {
			this->vtable_len++;
		}

		if (copy_vtable) {
			this->copy_vtable();
		}
	}

	void copy_vtable() {
		if (this->vtable_copy != nullptr) return;

#ifdef _WIN32
		// vtables are laid out in memory like
		//   [-1]: MSVC++ CompleteObjectLocator for RTTI
		//   [ 0]: ptr to virt fn 1
		//   [ 1]: ptr to virt fn 2
		//   ...
		//   [ n]: ptr to virt fn n
		//
		// so we need to allocate n+1 words and copy the completeobjectlocator before the start of the vtable too

		this->vtable_copy = std::make_unique<uintptr_t[]>(this->vtable_len + 1);
		std::memcpy(this->vtable_copy.get(),
			this->vtable_og - 1,
			(this->vtable_len + 1) * sizeof(uintptr_t));
#else
#error "unimplemented!: vtable copying on linux"
#endif
	}

	inline void _check_idx(size_t idx) const {
#ifdef IFACE_VTABLE_BOUNDS_CHECKS
		assert(idx >= 0 && idx < this->vtable_len);
#endif
	}

	//Interface() : baseptr(nullptr), vtable_og(nullptr), vtable_len(0) {}

	//! given a module and interface id symbol, find and run CreateInterface(iface_sym),
	//! check for errors, and return the created interface (or std::nullopt).
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
