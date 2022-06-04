#include "services.h"

#include "dbgcon.h"

DbgCon* Services::s_dbgcon = nullptr;
void Services::unload() {
	// explicit management because, yeah, unloading in source is weird.

	if (Services::s_dbgcon) delete s_dbgcon;
}