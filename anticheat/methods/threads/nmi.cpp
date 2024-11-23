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

	results = ExAllocatePool(NonPagedPool, PAGE_SIZE);
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

bool nmi::scan(int count)
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

void nmi::fire_nmi(int core, PKAFFINITY_EX mask)
{
	KeInitializeAffinityEx(mask);
	KeAddProcessorAffinityEx(mask, core);
	HalSendNMI(mask);
}

BOOLEAN nmi::callback(PVOID context, BOOLEAN handled)
{
	printf("NMI fired\n");

	return true;
}
