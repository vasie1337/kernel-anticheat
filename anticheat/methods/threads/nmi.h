#pragma once
#include "../../include.h"

class nmi
{
public:
	void init();

	/// <summary>
	/// Scan cores for suspicious activity by firing NMI
	/// </summary>
	/// <param name="count"> 
	/// Number of times each core should be scanned
	/// </param>
	/// <returns>
	/// True if successful, false otherwise
	/// </returns>
	bool scan(int count);

private:
	void fire_nmi(int core, PKAFFINITY_EX mask);
	
	static BOOLEAN callback(PVOID context, BOOLEAN handled);

	ULONG core_count = 0;
	ULONG context_size = 0;

	PNMI_CONTEXT context = nullptr;
	PKAFFINITY_EX mask = nullptr;
	void* results = nullptr;
	HANDLE nmi_handle = nullptr;

	bool did_init = false;
};