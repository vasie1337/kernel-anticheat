#pragma once
#include "../../include.h"

class nmi
{
public:
	void init();

	bool fire(int count);
	void scan();

private:
	void fire_nmi(int core, PKAFFINITY_EX mask);
	
	static void capture_stack();
	static BOOLEAN callback(PVOID context, BOOLEAN handled);

	ULONG core_count = 0;
	ULONG context_size = 0;

	PNMI_CONTEXT context = nullptr;
	PKAFFINITY_EX mask = nullptr;
	inline static ULONG64* results = nullptr;
	HANDLE nmi_handle = nullptr;

	bool did_init = false;
	inline static bool nmi_handled = false;
};

inline nmi g_nmi;