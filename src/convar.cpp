#pragma once

#include "convar.h"
#include "services.h"
#include "modules/tier1.h"

std::vector<CommandWrapper*>& CommandWrapper::GetList() {
	static std::vector<CommandWrapper*> list;
	return list;
}

CommandWrapper::CommandWrapper(const char* pName, FnCommandCallback_t callback, const char* pHelpString, int flags, FnCommandCompletionCallback completionFunc)
	: registered(false)
{
	this->inner = new ConCommand(pName, callback, pHelpString, flags, completionFunc);

	CommandWrapper::GetList().push_back(this);
}

void CommandWrapper::Register() {
	if (!this->registered) {
		auto tier1 = Services::get<Tier1>();
		// le funny vtable gank
		*(void**)this->inner = tier1->vt_ConCommand;

		tier1->RegisterConCommand(this->inner);
	}
	this->registered = true;
}

void CommandWrapper::RegisterAll() {
	for (auto cmd : CommandWrapper::GetList()) {
		cmd->Register();
	}
}
