#include "dbgcon.h"

#include <cassert>

DbgCon::DbgCon() {
	AllocConsole();

	this->hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	assert(this->hConsoleOutput != INVALID_HANDLE_VALUE);

	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	GetConsoleScreenBufferInfo(this->hConsoleOutput, &coninfo);
	coninfo.dwSize.X = 150;
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize(this->hConsoleOutput, coninfo.dwSize);
	SetConsoleTitleA("snp debug console");

	auto hConWin = GetConsoleWindow();
}

DbgCon::~DbgCon() {
	FreeConsole();
}

void DbgCon::log(const std::string& text) {
	if (this->hConsoleOutput != INVALID_HANDLE_VALUE) {
		WriteConsoleA(hConsoleOutput, text.c_str(), text.length(), NULL, NULL);
	}
}
