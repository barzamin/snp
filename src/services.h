#pragma once

#include <cassert>

#define SERVICE(type)              \
	public:                        \
	static void provide(type* x) { \
        s_##type = x;               \
	}                              \
    template<> static type* get() {\
		assert(s_##type != nullptr);\
        return s_##type;            \
    }                              \
    private:                       \
        static type* s_##type;

class DbgCon;
class Game;

class Services {
public:
	template<typename T> static T* get();
	static void unload();

	SERVICE(DbgCon)
	SERVICE(Game)
};

#undef SERVICE