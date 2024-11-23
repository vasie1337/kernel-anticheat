#include "nmi.h"

void nmi::init()
{
	core_count = KeQueryActiveProcessorCountEx(0);
	context_size = core_count * sizeof(NMI_CONTEXT);

	context = reinterpret_cast<PNMI_CONTEXT>(ExAllocatePool(NonPagedPool, context_size));
	if (!context)
	{
		return;
	}

	mask = reinterpret_cast<PKAFFINITY_EX>(ExAllocatePool(NonPagedPool, sizeof(KAFFINITY_EX)));
	if (!mask)
	{
		ExFreePool(context);
		return;
	}

	results = reinterpret_cast<ULONG64*>(ExAllocatePool(NonPagedPool, PAGE_SIZE));
	if (!results)
	{
		ExFreePool(mask);
		ExFreePool(context);
		return;
	}

	RtlZeroMemory(context, context_size);
	RtlZeroMemory(mask, sizeof(KAFFINITY_EX));
	RtlZeroMemory(results, PAGE_SIZE);

	did_init = true;
}

bool nmi::fire(int count)
{
	if (!did_init)
	{
		printf("NMI not initialized\n");
		return false;
	}

	nmi_handle = KeRegisterNmiCallback(callback, nullptr);
	if (!nmi_handle)
	{
		printf("Failed to register NMI callback\n");
		return false;
	}

	LARGE_INTEGER delay = { 0 };
	delay.QuadPart = -(10 * 100); // 10ms

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < core_count; j++)
		{
			fire_nmi(j, mask);
			KeDelayExecutionThread(KernelMode, FALSE, &delay);
		}
	}

	KeDeregisterNmiCallback(nmi_handle);

	return true;
}

void nmi::scan()
{
	bool found = false;

	for (INT i = 0; i < 0x1000 / 0x10; i += 2)
	{
		if (results[i] == 0)
			continue;

		if (MmIsAddressValid(reinterpret_cast<void*>(results[i])) && results[i + 1])
		{
			for (USHORT j = 0; j < results[i + 1]; j++)
			{
				uintptr_t address = (((DWORD64**)results)[i])[j];

				if (address < 0xFFFF000000000000)
					break;

				if (address_outside_modules(address))
				{
					printf("[NMI] Address outside of module list: 0x%llx\n", address);
					found = true;
				}
			}
		}

		ExFreePool(reinterpret_cast<void*>(results[i]));
		results[i] = 0;
		results[i + 1] = 0;
	}

	if (!found)
	{
		printf("[NMI] No addresses outside of module list found\n");
	}
}

void nmi::fire_nmi(int core, PKAFFINITY_EX mask)
{
	KeInitializeAffinityEx(mask);
	KeAddProcessorAffinityEx(mask, core);
	HalSendNMI(mask);
}

void nmi::capture_stack()
{
	void** stack_trace = reinterpret_cast<void**>(ExAllocatePool(NonPagedPool, PAGE_SIZE));
	if (!stack_trace)
	{
		printf("Failed to allocate memory for stack trace\n");
		return;
	}

	USHORT frames = RtlCaptureStackBackTrace(0, PAGE_SIZE / 8, stack_trace, 0);
	if (!frames)
	{
		ExFreePool(stack_trace);
		return;
	}

	for (int i = 0; i < 0x1000 / 0x10; i += 2)
	{
		if (results[i] == 0)
		{
			results[i] = reinterpret_cast<ULONG64>(stack_trace);
			results[i + 1] = frames;
			break;
		}
	}

	printf("Stack trace captured\n");
}

BOOLEAN nmi::callback(PVOID context, BOOLEAN handled)
{
	printf("NMI fired\n");

	capture_stack();

	return true;
}
