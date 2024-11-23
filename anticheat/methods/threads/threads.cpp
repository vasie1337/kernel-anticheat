#include "threads.h"

void threads::scan_system_threads()
{
	bool found = false;

	for (ULONG thrd_id = 4; thrd_id < 0x30000; thrd_id += 4)
	{
		PETHREAD thread_obj = nullptr;
		if (!NT_SUCCESS(PsLookupThreadByThreadId(reinterpret_cast<HANDLE>(thrd_id), &thread_obj)))
			continue;

		if (!PsIsSystemThread(thread_obj) || thread_obj == KeGetCurrentThread())
			continue;

		uintptr_t start_addr = get_thread_start_address(thread_obj);
		if (start_addr && address_outside_modules(start_addr))
		{
			printf("[THREADS] Startaddress not valid : %llx\n", start_addr);
			found = true;
		}
	}

	if (!found)
	{
		printf("[THREADS] No addresses outside of module list found\n");
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