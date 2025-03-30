#pragma once

#ifdef __SWITCH__
#include "../Platform.hpp" // A bit lazy to set this up correctly
#else
#include "Utils/Platform.hpp"
#endif
#include "UtlMemory.hpp"
#include "Color.hpp"

#define FCVAR_NONE                       0
#define FCVAR_UNREGISTERED               (1 << 0)
#define FCVAR_DEVELOPMENTONLY            (1 << 1)
#define FCVAR_GAMEDLL                    (1 << 2)
#define FCVAR_CLIENTDLL                  (1 << 3)
#define FCVAR_HIDDEN                     (1 << 4)
#define FCVAR_PROTECTED                  (1 << 5)
#define FCVAR_SPONLY                     (1 << 6)
#define	FCVAR_ARCHIVE                    (1 << 7)
#define	FCVAR_NOTIFY                     (1 << 8)
#define	FCVAR_USERINFO                   (1 << 9)
#define FCVAR_PRINTABLEONLY              (1 << 10)
#define FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS (1 << 10)
#define FCVAR_UNLOGGED                   (1 << 11)
#define FCVAR_NEVER_AS_STRING            (1 << 12)
#define FCVAR_REPLICATED                 (1 << 13)
#define FCVAR_CHEAT                      (1 << 14)
#define FCVAR_SS                         (1 << 15)
#define FCVAR_DEMO                       (1 << 16)
#define FCVAR_DONTRECORD                 (1 << 17)
#define FCVAR_SS_ADDED                   (1 << 18)
#define FCVAR_RELEASE                    (1 << 19)
#define FCVAR_RELOAD_MATERIALS           (1 << 20)
#define FCVAR_RELOAD_TEXTURES            (1 << 21)
#define FCVAR_NOT_CONNECTED              (1 << 22)
#define FCVAR_MATERIAL_SYSTEM_THREAD     (1 << 23)
#define FCVAR_ARCHIVE_GAMECONSOLE        (1 << 24)
#define FCVAR_ACCESSIBLE_FROM_THREADS    (1 << 25)
#define FCVAR_SERVER_CAN_EXECUTE         (1 << 28)
#define FCVAR_SERVER_CANNOT_QUERY        (1 << 29)
#define FCVAR_CLIENTCMD_CAN_EXECUTE      (1 << 30)

#define COMMAND_COMPLETION_MAXITEMS 64
#define COMMAND_COMPLETION_ITEM_LENGTH 64

struct CCommand;
struct ConCommandBase;

typedef void ( *FnChangeCallback_t )( void *var, const char *pOldValue, float flOldValue );

using _CommandCallback = void (*)(const CCommand &args);
using _CommandCompletionCallback = int (*)(const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);
using _InternalSetValue = void(__rescalll *)(void *thisptr, const char *value);
using _InternalSetFloatValue = void(__rescalll *)(void *thisptr, float value);
using _InternalSetIntValue = void(__rescalll *)(void *thisptr, int value);
using _RegisterConCommand = void(__rescalll *)(void *thisptr, ConCommandBase *pCommandBase);
using _UnregisterConCommand = void(__rescalll *)(void *thisptr, ConCommandBase *pCommandBase);
using _FindCommandBase = void *(__rescalll *)(void *thisptr, const char *name);
using _InstallGlobalChangeCallback = void(__rescalll *)(void *thisptr, FnChangeCallback_t callback);
using _RemoveGlobalChangeCallback = void(__rescalll *)(void *thisptr, FnChangeCallback_t callback);

class IConVar {
public:
	virtual void SetValue(const char *pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;
	virtual void SetValue(Color value) = 0;
	virtual const char *GetName(void) const = 0;
	virtual const char *GetBaseName(void) const = 0;
	virtual bool IsFlagSet(int nFlag) const = 0;
	virtual int GetSplitScreenPlayerSlot() const = 0;
};

struct ConCommandBase {
	ConCommandBase( const char *name, int flags, const char *helpstr )
		: m_pNext(nullptr)
		, m_bRegistered(false)
		, m_pszName(name)
		, m_pszHelpString(helpstr)
		, m_nFlags(flags) {
	}

#ifdef __SWITCH__
	virtual		~ConCommandBase();
#else
	// if we actually put a virtual destructor here, EVERYTHING BREAKS
	// so put dummy methods instead
	virtual void _dtor(){};
#ifndef _WIN32
	virtual void _dtor1(){};
#endif
#endif
	virtual bool IsCommand() const { return false; };  // will get overwritten anyway lol
	// Note: vtable incomplete, but sufficient

	ConCommandBase *m_pNext;      // 4	(Switch: 8)
	bool m_bRegistered;           // 8	(Switch: 16)
	const char *m_pszName;        // 12	(Switch: 24)
	const char *m_pszHelpString;  // 16	(Switch: 32)
	int m_nFlags;                 // 20	(Switch: 40)
};

struct characterset_t;

struct CCommand {
	enum {
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512
	};
	int m_nArgc;
	int m_nArgv0Size;
	char m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char *m_ppArgv[COMMAND_MAX_ARGC];

	int ArgC() const {
		return this->m_nArgc;
	}
	const char *Arg(int nIndex) const {
		return this->m_ppArgv[nIndex];
	}
	const char *operator[](int nIndex) const {
		return Arg(nIndex);
	}
};

struct ConCommand : ConCommandBase {
	union {
		void *m_fnCommandCallbackV1;
		_CommandCallback m_fnCommandCallback;
		void *m_pCommandCallback;
	};

	union {
		_CommandCompletionCallback m_fnCompletionCallback;
		void *m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback : 1;
	bool m_bUsingNewCommandCallback : 1;
	bool m_bUsingCommandCallbackInterface : 1;

	ConCommand(const char *pName, _CommandCallback callback, const char *pHelpString, int flags = 0, _CommandCompletionCallback completionFunc = nullptr)
		: ConCommandBase(pName, flags, pHelpString)
		, m_fnCommandCallback(callback)
		, m_fnCompletionCallback(completionFunc)
		, m_bHasCompletionCallback(completionFunc != nullptr)
		, m_bUsingNewCommandCallback(true)
		, m_bUsingCommandCallbackInterface(false) {
	}
};

struct ConVar : ConCommandBase {
	void *ConVar_VTable;                                // 24 (Switch: 48)
	ConVar *m_pParent;                                  // 28 (Switch: 56)
	const char *m_pszDefaultValue;                      // 32 (Switch: 64)
	char *m_pszString;                                  // 36 (Switch: 72)
	int m_StringLength;                                 // 40 (Switch: 80)
	float m_fValue;                                     // 44 (Switch: 84)
	int m_nValue;                                       // 48 (Switch: 88)
	bool m_bHasMin;                                     // 52 (Switch: 96)
	float m_fMinVal;                                    // 56 (Switch: 100)
	bool m_bHasMax;                                     // 60 (Switch: 104)
	float m_fMaxVal;                                    // 64 (Switch: 108)
	CUtlVector<FnChangeCallback_t> m_fnChangeCallback;  // 68 (Switch: 112)

	ConVar(const char *name, const char *value, int flags, const char *helpstr, bool hasmin = false, float min = 0.0, bool hasmax = false, float max = 0.0)
		: ConCommandBase(name, flags, helpstr)
		, m_pParent(nullptr)
		, m_pszDefaultValue(value)
		, m_pszString(nullptr)
		, m_StringLength(0)
		, m_fValue(0.0f)
		, m_nValue(0)
		, m_bHasMin(hasmin)
		, m_fMinVal(min)
		, m_bHasMax(hasmax)
		, m_fMaxVal(max)
		, m_fnChangeCallback() {
	}
};

#define FOR_ALL_CONSOLE_COMMANDS( pCommandVarName ) \
	ConCommandBase *m_pConCommandList = *reinterpret_cast<ConCommandBase **>((uintptr_t)g_pCVar + 80); /* CCvar::m_pConCommandList */ \
	for (ConCommandBase *pCommandVarName = m_pConCommandList; pCommandVarName; pCommandVarName = *reinterpret_cast<ConCommandBase **>(reinterpret_cast<uintptr_t >(pCommandVarName) + 8)) /* ConCommandBase::m_pNext (private variable) */

