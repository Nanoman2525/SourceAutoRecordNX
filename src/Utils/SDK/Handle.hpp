#pragma once

#ifndef __SWITCH__
#include "Offsets.hpp" // TODO: Implement
#endif

class CBaseHandle {
	friend class EntityList;

public:
#ifndef __SWITCH__
	// TODO: Implement
	inline int GetEntryIndex() const {
		return m_Index & Offsets::ENT_ENTRY_MASK;
	}

	inline int GetSerialNumber() const {
		return m_Index >> Offsets::NUM_SERIAL_NUM_SHIFT_BITS;
	}
#endif

	unsigned long m_Index;

	bool operator !() const {
		return m_Index == 0xFFFFFFFF;
	}

	operator bool() const {
		return !!*this;
	}

	// specify non-trivial copy constructor to get correct arg-passing behaviour
	CBaseHandle(const CBaseHandle &other) {
		this->m_Index = other.m_Index;
	}

	CBaseHandle() {
		this->m_Index = 0xFFFFFFFF;
	}
};

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle &GetRefEHandle() const = 0;
};
