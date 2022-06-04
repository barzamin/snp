#include "game.h"
#include "hl2_source_unpack.h"

HL2SourceUnpack::HL2SourceUnpack() {
}

const char* HL2SourceUnpack::name() {
	return "Half-Life 2 (build 5135) [Source Unpack]";
}

Offsets HL2SourceUnpack::s_offsets = {
	.ICVar_RegisterConCommand = 6,
	.ICVar_UnregisterConCommand = 7,
};
