#pragma once

#include <exception>

//class Module {
//public:
//	virtual ~Module() = default;
//	virtual bool init() = 0;
//	virtual void shutdown() = 0;
//};

class ModuleLoadError : public std::exception {
public:
	const char* what() const noexcept {
		return "module load error";
	}
};
