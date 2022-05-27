#pragma once

#include <cmath>
#include <cstdint>

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

// Used internally to register classes.
struct InterfaceReg {
	InstantiateInterfaceFn	m_CreateFn;
	const char* m_pName;

	InterfaceReg* m_pNext; // For the global list.
	static InterfaceReg* s_pInterfaceRegs;

	InterfaceReg(InstantiateInterfaceFn fn, const char* pName)
		: m_CreateFn{ fn },
		m_pName{ pName }
	{
		m_pNext = s_pInterfaceRegs;
		s_pInterfaceRegs = this;
	}
};

// interface return status
enum
{
	IFACE_OK = 0,
	IFACE_FAILED
};

// Use this to expose an interface that can have multiple instances.
// e.g.:
// EXPOSE_INTERFACE( CInterfaceImp, IInterface, "MyInterface001" )
// This will expose a class called CInterfaceImp that implements IInterface (a pure class)
// clients can receive a pointer to this class by calling CreateInterface( "MyInterface001" )
//
// In practice, the shared header file defines the interface (IInterface) and version name ("MyInterface001")
// so that each component can use these names/vtables to communicate
//
// A single class can support multiple interfaces through multiple inheritance
//
// Use this if you want to write the factory function.
#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
	static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);

#define EXPOSE_INTERFACE(className, interfaceName, versionName) \
	static void* __Create##className##_interface() {return static_cast<interfaceName *>( new className );} \
	static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName );

// Use this to expose a singleton interface with a global variable you've created.
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR_WITH_NAMESPACE(className, interfaceNamespace, interfaceName, versionName, globalVarName) \
	static void* __Create##className##interfaceName##_interface() {return static_cast<interfaceNamespace interfaceName *>( &globalVarName );} \
	static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName) \
	EXPOSE_SINGLE_INTERFACE_GLOBALVAR_WITH_NAMESPACE(className, , interfaceName, versionName, globalVarName)

#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
	static className __g_##className##_singleton; \
	EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)

// via convar.h
struct CCommand {
	enum
	{
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512,
	};

	int		m_nArgc;
	int		m_nArgv0Size;
	char	m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char	m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];

	int ArgC() const
	{
		return m_nArgc;
	}

	const char* Arg(int nIndex) const
	{
		// FIXME: Many command handlers appear to not be particularly careful
		// about checking for valid argc range. For now, we're going to
		// do the extra check and return an empty string if it's out of range
		if (nIndex < 0 || nIndex >= m_nArgc)
			return "";
		return m_ppArgv[nIndex];
	}

	const char* operator[](int nIndex) const
	{
		return Arg(nIndex);
	}
};

struct CBaseEdict {
	int	m_fStateFlags;
	// #if VALVE_LITTLE_ENDIAN
	short m_NetworkSerialNumber;
	short m_EdictIndex;
	void* m_pNetworkable; // IServerNetworkable*
	void* m_pUnk; // IServerUnknown*
};

//-----------------------------------------------------------------------------
// Purpose: The engine's internal representation of an entity, including some
//  basic collision and position info and a pointer to the class wrapped on top
//  of the structure
//-----------------------------------------------------------------------------
struct edict_t : public CBaseEdict {};

typedef enum
{
	PLUGIN_CONTINUE = 0, // keep going
	PLUGIN_OVERRIDE, // run the game dll function but use our return value instead
	PLUGIN_STOP, // don't run the game dll function at all
} PLUGIN_RESULT;

typedef enum
{
	eQueryCvarValueStatus_ValueIntact = 0,	// It got the value fine.
	eQueryCvarValueStatus_CvarNotFound = 1,
	eQueryCvarValueStatus_NotACvar = 2,		// There's a ConCommand, but it's not a ConVar.
	eQueryCvarValueStatus_CvarProtected = 3	// The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
} EQueryCvarValueStatus;

typedef int QueryCvarCookie_t;
#define InvalidQueryCvarCookie -1

#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS "ISERVERPLUGINCALLBACKS002"

class IServerPluginCallbacks
{
public:
	// Initialize the plugin to run
	// Return false if there is an error during startup.
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
	// Called when the plugin should be shutdown
	virtual void			Unload(void) = 0;
	// called when a plugins execution is stopped but the plugin is not unloaded
	virtual void			Pause(void) = 0;
	// called when a plugin should start executing again (sometime after a Pause() call)
	virtual void			UnPause(void) = 0;
	// Returns string describing current plugin.  e.g., Admin-Mod.  
	virtual const char* GetPluginDescription(void) = 0;
	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	virtual void			LevelInit(char const* pMapName) = 0;
	// The server is about to activate
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) = 0;
	// The server should run physics/think on all edicts
	virtual void			GameFrame(bool simulating) = 0;
	// Called when a level is shutdown (including changing levels)
	virtual void			LevelShutdown(void) = 0;
	// Client is going active
	virtual void			ClientActive(edict_t* pEntity) = 0;
	// Client is disconnecting from server
	virtual void			ClientDisconnect(edict_t* pEntity) = 0;
	// Client is connected and should be put in the game
	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) = 0;
	// Sets the client index for the client who typed the command into their console
	virtual void			SetCommandClient(int index) = 0;
	// A player changed one/several replicated cvars (name etc)
	virtual void			ClientSettingsChanged(edict_t* pEdict) = 0;
	// Client is connecting to server ( set retVal to false to reject the connection )
	//	You can specify a rejection message by writing it into reject
	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;
	// The client has typed a command at the console
	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args) = 0;
	// A user has had their network id setup and validated 
	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;
	// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
	// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
	// Added with version 2 of the interface.
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) = 0;
	// added with version 3 of the interface.
	virtual void			OnEdictAllocated(edict_t* edict) = 0;
	virtual void			OnEdictFreed(const edict_t* edict) = 0;
};

// reimplemtning mathlib/vector.h
struct Vector {
	float x, y, z; // typedef float vec_t
	Vector() : x{ 0 }, y{ 0 }, z{ 0 } {}
	Vector(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}

	float LengthSqr() const {
		return x * x + y * y + z * z;
	}

	float Length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance);
	}

	float Dot(const Vector& other) const {
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	Vector operator*(float scale) const {
		return { this->x * scale, this->y * scale, this->z * scale };
	}

	Vector operator/(float divisor) const {
		// yes, this is *not* the same thing as division when using IEEE float.
		// however, this is what mathlib/vector.h does...
		return *this * (1.0f / divisor);
	}

	Vector operator+(const Vector& other) const {
		return { this->x + other.x, this->y + other.y, this->z + other.z };
	}

	Vector operator-(const Vector& other) const {
		return { this->x - other.x, this->y - other.y, this->z - other.z };
	}

	Vector operator-() const {
		return { -this->x, -this->y, -this->z };
	}

	Vector Cross(const Vector& other) const {
		return {
			this->y * other.z - this->z * other.y,
			this->z * other.x - this->x * other.z,
			this->x * other.y - this->y * other.x,
		};
	}

	Vector Normalized() const {
		// not technically what source does, but it depends on what vector extensions a source build was targeting anyway
		return *this / this->Length();
	}
};

struct Color {
	uint8_t r, g, b, a;

	Color()
		: Color(0, 0, 0) {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
		: r{ r }, g{ g }, b{ b }, a{ a } {}
};
