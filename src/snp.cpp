#include "snp.h"

#include <format>
#include <memory>
#include <spdlog/spdlog.h>

#include "sdk.h"
#include "mem.h"
#include "interface.h"
#include "platform.h"
#include "services.h"
#include "games/game.h"
#include "modules/tier1.h"
#include "modules/console.h"
#include "convar.h"

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
	Services::provide(new DbgCon());
	auto sink = std::make_shared<dbgcon_sink_mt>();
	auto logger = std::make_shared<spdlog::logger>("snp", sink);
	spdlog::set_default_logger(logger);
	spdlog::set_level(spdlog::level::debug);

	spdlog::info("logger initialized");

	auto game = Game::identify();
	if (game != nullptr) {
		Services::provide(game);
		spdlog::info("game: {}", game->name());

		spdlog::info("loaded snp (build " SNP_BUILDSTAMP ")");

		try {
			auto tier1 = new Tier1(*game);
			spdlog::info("tier1 services initialized");
			Services::provide(tier1);

			auto console = new Console(*tier1);
			spdlog::info("console services initialized");
			Services::provide(console);

			spdlog::info("registering all concommands");
			CommandWrapper::RegisterAll();

			return true; // succ !
		} catch ([[maybe_unused]] const ModuleLoadError& e) {
			spdlog::error("module load error! terminating");
			return false;
		}
	}

	return false; // unsuccessful startup; engine will fire Unload
}

void SNP::Unload()
{
	Services::unload();
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

CON_COMMAND(snp_about, "snp_about - prints info about SNP\n") {
	auto info = std::format("snp (build " SNP_BUILDSTAMP ") is loaded and running.\n");
	spdlog::info(info);
	Services::get<Console>()->log(info.c_str());
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
