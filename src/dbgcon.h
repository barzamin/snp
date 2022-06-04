#pragma once

#include <windows.h>
#include <string>
#include <format>
#include <mutex>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/details/null_mutex.h>

#include "services.h"

class DbgCon {
private:
	HANDLE hConsoleOutput;

public:
	DbgCon();
	~DbgCon();
	DbgCon(const DbgCon&) = delete;
	DbgCon(DbgCon&&) = default;

	void log(const std::string& text);

};

template<typename Mutex>
class dbgcon_sink : public spdlog::sinks::base_sink<Mutex> {
public:
	dbgcon_sink() {}

protected:
	void sink_it_(const spdlog::details::log_msg& msg) override {
		std::string formatted;
		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
		Services::get<DbgCon>()->log(formatted);
	}

	void flush_() override {}
};

using dbgcon_sink_mt = dbgcon_sink<std::mutex>;
using dbgcon_sink_st = dbgcon_sink<spdlog::details::null_mutex>;
