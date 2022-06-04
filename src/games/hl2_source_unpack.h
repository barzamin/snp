#pragma once

#include "game.h"

class HL2SourceUnpack : public Game {
public:
	HL2SourceUnpack();

	Offsets* offsets() override {
		return &s_offsets;
	}

	const char* name() override;
	GameFlavor flavor() override {
		return GameFlavor::HL2SourceUnpack;
	}

private:
	static Offsets s_offsets;
};
