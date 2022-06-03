#pragma once

#include <windows.h>
#include <string>

class DbgCon {
private:
	HANDLE hConsoleOutput;

public:
	DbgCon();
	~DbgCon();

	void log(const std::string& text);
};
