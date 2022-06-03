#pragma once

#include "module.h"
#include "../interface.h"

#include <string>
#include <memory>


class Tier1 : Module {
public:
	bool init() override;
	void shutdown() override;

private:
	std::unique_ptr<Interface> inner;
};
