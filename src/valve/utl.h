// valve's STL replacements
#pragma once

#include <cassert>
#include <cstdlib>

template<class T, class I = int>
struct CUtlMemory {
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;

	int NumAllocated() const { return m_nAllocationCount; }
	void Grow(int num) {
		assert(num > 0);

		// TODO: make this more robust/match utlmemory.h better
		m_nAllocationCount += num;
		m_pMemory = static_cast<T*>(realloc(m_pMemory, sizeof(T) * m_nAllocationCount));
	}

	T* Base() { return m_pMemory; }
	const T* Base() const { return m_pMemory; }

	void Purge() {
		if (m_pMemory) {
			free(m_pMemory);
			m_pMemory = nullptr;
		}
		m_nAllocationCount = 0;
	}
};

template< class T, class A = CUtlMemory<T> >
class CUtlVector {
	A m_Memory;
	int m_Size;

	T* Base() { return m_Memory.Base(); }
	const T* Base() const { return m_Memory.Base(); }

	T* m_pElements;
	inline void ResetDbgInfo() {
		m_pElements = this->Base();
	}

	void GrowVector(int num = 1) {
		if (m_Size + num > m_Memory.NumAllocated()) {
			m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());
		}

		m_Size += num;
		ResetDbgInfo();
	}

	// retval technically not matching, but trying to be faithful to valve utl api
	void AddToTail(const T& src) {
		this->Base()[this->m_Size] = src;
		this->mSize++;
	}

	// equiv to sar's Clear. *does not call dtors*, so does *not* have parity with valve api 
	void Purge() {
		m_Memory.Purge();
		ResetDbgInfo();
	}
};

// simple string class. "only for tools", says valve, not in runtime code (and uses it in runtime code anyway)
class CUtlString {
public:
	const char* Get() const {
		if (!m_pString)
			return "";
		return m_pString;
	}

private:
	char* m_pString;
};
