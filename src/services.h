#pragma once

#include <cassert>

class DbgCon;

class Services {
public:
	static void provide(DbgCon* x) {
		s_dbgcon = x;
	}

	template<typename T> static T* get();
	template<> static DbgCon* get() {
		assert(s_dbgcon != nullptr);
		return s_dbgcon;
	}

	static void unload();

private:
	static DbgCon* s_dbgcon;
};

