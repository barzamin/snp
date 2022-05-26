#include "snp.h"

SNP snp;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(
	SNP,
	IServerPluginCallbacks,
	INTERFACEVERSION_ISERVERPLUGINCALLBACKS,
	snp);

SNP::SNP() {
	// blah ctor blah blah
}

bool SNP::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	return true; // successful startup
}

void SNP::Unload()
{
}

void SNP::Pause()
{
}

void SNP::UnPause()
{
}

const char* SNP::GetPluginDescription()
{
	return "SourceNyoomPlugin";
}

void SNP::LevelInit(const char* pMapName)
{
}

void SNP::ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
}

void SNP::GameFrame(bool simulating)
{
}

#pragma region "unused callbacks"

PLUGIN_RESULT SNP::ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen)
{
	return PLUGIN_CONTINUE;
}

PLUGIN_RESULT SNP::ClientCommand(edict_t* pEntity, const CCommand& args)
{
	return PLUGIN_CONTINUE;
}

PLUGIN_RESULT SNP::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
{
	return PLUGIN_CONTINUE;
}


void SNP::LevelShutdown() {}

void SNP::ClientActive(edict_t* pEntity) {}

void SNP::ClientDisconnect(edict_t* pEntity) {}

void SNP::ClientPutInServer(edict_t* pEntity, const char* playername) {}

void SNP::SetCommandClient(int index) {}

void SNP::ClientSettingsChanged(edict_t* pEdict) {}

void SNP::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) {}

void SNP::OnEdictAllocated(edict_t* edict) {}

void SNP::OnEdictFreed(const edict_t* edict) {}

#pragma endregion