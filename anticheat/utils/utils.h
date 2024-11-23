#pragma once
#include "../include.h"

inline bool address_outside_modules(uintptr_t address)
{
	BOOLEAN outside_modulelist = TRUE;
	PSYSTEM_MODULE_INFORMATION system_info_buffer = nullptr;
	ULONG system_info_buffer_size = 0;

	ZwQuerySystemInformation(SystemModuleInformation, 0, 0, &system_info_buffer_size);

	system_info_buffer = (PSYSTEM_MODULE_INFORMATION)ExAllocatePool(NonPagedPool, (SIZE_T)system_info_buffer_size * 2);
	if (!system_info_buffer)
	{
		return false;
	}

	memset(system_info_buffer, 0, (SIZE_T)system_info_buffer_size * 2);
	ZwQuerySystemInformation(SystemModuleInformation, system_info_buffer, (SIZE_T)system_info_buffer_size * 2, &system_info_buffer_size);

	for (ULONG i = 0; i < system_info_buffer->Count; i++)
	{
		if (address >= (ULONG64)system_info_buffer->Module[i].ImageBase &&
			address <= (ULONG64)system_info_buffer->Module[i].ImageBase + system_info_buffer->Module[i].ImageSize)
		{
			outside_modulelist = FALSE;
			break;
		}
	}

	ExFreePool(system_info_buffer);

	return outside_modulelist;
}