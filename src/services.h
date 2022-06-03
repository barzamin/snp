#pragma once

#include <cassert>

#include "dbgcon.h"


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

	static void unload() {
		// explicit management because, yeah, unloading in source is weird.

		if (s_dbgcon) delete s_dbgcon;
	}

private:
	static DbgCon* s_dbgcon;
};

