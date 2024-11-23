#include "threads.h"

void threads::scan_system_threads()
{
	for (ULONG thrd_id = 4; thrd_id < 0x30000; thrd_id += 4)
	{
		PETHREAD thread_obj = nullptr;
		if (!NT_SUCCESS(PsLookupThreadByThreadId(reinterpret_cast<HANDLE>(thrd_id), &thread_obj)))
			continue;

		if (!PsIsSystemThread(thread_obj) || thread_obj == KeGetCurrentThread())
			continue;

		uintptr_t start_addr = get_thread_start_address(thread_obj);
		if (start_addr && address_outside_modules(start_addr))
			DbgPrint("Startaddress not valid : %llx", start_addr);
			
	}
}

uintptr_t threads::get_thread_start_address(PETHREAD thread)
{
	HANDLE thread_handle = nullptr;

	if (!NT_SUCCESS(ObOpenObjectByPointer(thread, OBJ_KERNEL_HANDLE, NULL, GENERIC_READ, *PsThreadType, KernelMode, &thread_handle)))
	{
		return 0;
	}

	uintptr_t start_addr;
	ULONG returned_bytes;
	if (!NT_SUCCESS(NtQueryInformationThread(thread_handle, ThreadQuerySetWin32StartAddress, &start_addr, sizeof(start_addr), &returned_bytes)))
	{
		NtClose(thread_handle);
		return 0;
	}

	if (!MmIsAddressValid((void*)start_addr))
	{
		NtClose(thread_handle);
		return 0;
	}

	NtClose(thread_handle);

	return start_addr;
}

bool threads::address_outside_modules(uintptr_t address)
{
	BOOLEAN outside_modulelist = TRUE;
	PSYSTEM_MODULE_INFORMATION system_info_buffer = nullptr;
	ULONG system_info_buffer_size = 0;

	ZwQuerySystemInformation(SystemModuleInformation, 0, 0, &system_info_buffer_size);

	system_info_buffer = (PSYSTEM_MODULE_INFORMATION)ExAllocatePool(NonPagedPool, (SIZE_T)system_info_buffer_size * 2);

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
