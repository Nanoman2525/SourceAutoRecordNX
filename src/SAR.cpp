#include "SAR.hpp"

#include "Utils/Platform.hpp"
#include "Utils/SDK/ServerPlugin.hpp"

#ifdef __SWITCH__
#include "NXModule.hpp"
#endif

#include <cstring>

#define SAR_PLUGIN_SIGNATURE "SourceAutoRecordNX"

void *g_pCVar = nullptr;

SAR sar;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(SAR, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, sar);

bool SAR::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
#ifdef __SWITCH__
	// base of all modules shipped
	if (!InitNXModuleBases()) {
		return false;
	}

	g_pCVar = interfaceFactory("VEngineCvar007", nullptr);
#endif

	return true;
}

#pragma region Unused callbacks
void SAR::Unload() {
}
void SAR::Pause() {
}
void SAR::UnPause() {
}
const char *SAR::GetPluginDescription() {
	return SAR_PLUGIN_SIGNATURE;
}
void SAR::LevelInit(char const *pMapName) {
}
void SAR::ServerActivate(void *pEdictList, int edictCount, int clientMax) {
}
void SAR::GameFrame(bool simulating) {
}
void SAR::LevelShutdown() {
}
void SAR::ClientFullyConnect(void *pEdict) {
}
void SAR::ClientActive(void *pEntity) {
}
void SAR::ClientDisconnect(void *pEntity) {
}
void SAR::ClientPutInServer(void *pEntity, char const *playername) {
}
void SAR::SetCommandClient(int index) {
}
void SAR::ClientSettingsChanged(void *pEdict) {
}
int SAR::ClientConnect(bool *bAllowConnect, void *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) {
	return 0;
}
int SAR::ClientCommand(void *pEntity, const void *&args) {
	return 0;
}
int SAR::NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) {
	return 0;
}
void SAR::OnQueryCvarValueFinished(int iCookie, void *pPlayerEntity, int eStatus, const char *pCvarName, const char *pCvarValue) {
}
void SAR::OnEdictAllocated(void *edict) {
}
void SAR::OnEdictFreed(const void *edict) {
}
#pragma endregion
