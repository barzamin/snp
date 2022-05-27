#pragma once

class Module {
public:
	virtual ~Module() = default;
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
	virtual const char* Name() = 0;
};
