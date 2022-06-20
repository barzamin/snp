#include "services.h"

#include "dbgcon.h"
#include "games/game.h"
#include "modules/tier1.h"

#define SERVICE(type) type* Services::s_##type = nullptr

SERVICE(DbgCon);
SERVICE(Game);
SERVICE(Tier1);

void Services::unload() {
	// explicit management because, yeah, unloading in source is weird.

	if (Services::s_DbgCon) delete s_DbgCon;
	if (Services::s_Game) delete s_Game;
	if (Services::s_Tier1) delete s_Tier1;
}