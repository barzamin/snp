#pragma once

#include "offsets.h"

enum class GameFlavor {
	HL2SourceUnpack,
};

class Game {
public:
	Game(const Game&) = delete;
	Game(Game&&) = default;

	virtual ~Game() = default;
	virtual Offsets* offsets() = 0;
	virtual const char* name() = 0;
	virtual GameFlavor flavor() = 0;

	static Game* identify();

protected:
	Game() = default;
};
