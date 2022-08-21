#pragma once

#include <vector>

#include "sdk.h"


class CommandWrapper {
private:
	ConCommand* inner;
	bool registered;

public:
	CommandWrapper(const char* pName,
		FnCommandCallback_t callback,
		const char* pHelpString,
		int flags = 0,
		FnCommandCompletionCallback completionFunc = nullptr);

	static std::vector<CommandWrapper*>& GetList();

	void Register();
	void Unregister();

	static void RegisterAll();
	static void UnregisterAll();
};

//-----------------------------------------------------------------------------
// Purpose: Utility macros to quicky generate a simple console command
//-----------------------------------------------------------------------------
#define CON_COMMAND( name, description ) \
   static void name( const CCommand &args ); \
   static CommandWrapper name##_command( #name, name, description ); \
   static void name( const CCommand &args )

#define CON_COMMAND_F( name, description, flags ) \
   static void name( const CCommand &args ); \
   static CommandWrapper name##_command( #name, name, description, flags ); \
   static void name( const CCommand &args )

#define CON_COMMAND_F_COMPLETION( name, description, flags, completion ) \
	static void name( const CCommand &args ); \
	static CommandWrapper name##_command( #name, name, description, flags, completion ); \
	static void name( const CCommand &args )

