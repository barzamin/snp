#pragma once

#include "module.h"
#include "../sdk.h"

class Game;
class Tier1;

#define SNP_LOG_COLOR Color(247, 214, 68, 0xff)

class Console {
public:
	Console(Tier1& tier1);
	void shutdown();

	Console(const Console&) = delete;
	Console(Console&&) = default;

	// checked thunks
	template<typename... T>
	void ConColorMsg(const Color& clr, const char* pMsgFormat, T... args) {
		assert(this->ConColorMsg_ptr != nullptr);
		this->ConColorMsg_ptr(clr, pMsgFormat, args...);
	}

	// utils
	template<typename... T>
	void log(const char* fmt, T... args) {
		this->ConColorMsg(SNP_LOG_COLOR, fmt, args...);
	}

private:
	using _ConColorMsg = void(__cdecl*)(const Color& clr, const char* pMsgFormat, ...);

	_ConColorMsg ConColorMsg_ptr = nullptr;
};
