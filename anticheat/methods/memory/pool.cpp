#include "pool.h"

#define TdlS_TAG 'SldT'
#define TdlS_OFFSET 0x184
#define TdlS_TIMESTAMP 0x0B024BC8B48

bool pool::scan_big_pool()
{
	PSYSTEM_BIGPOOL_INFORMATION info = get_big_pool_info();
	if (!info)
		return false;

	bool found = false;

	for (ULONG i = 0; i < info->Count; i++)
	{
		if (info->AllocatedInfo[i].TagULong != TdlS_TAG)
			return false;

		found = true;

		printf("[POOL] Found TdlS tag in big pool\n");

		void* page = MmMapIoSpaceEx(MmGetPhysicalAddress(reinterpret_cast<void*>(info->AllocatedInfo[i].VirtualAddress)), PAGE_SIZE, PAGE_READWRITE);
		if (reinterpret_cast<uintptr_t>(page) + TdlS_OFFSET == TdlS_TIMESTAMP)
		{
			printf("[POOL] TdlS timestamp found in pool\n");
		}

		MmUnmapIoSpace(page, PAGE_SIZE);
	}

	ExFreePool(info);

	if (!found)
		printf("[POOL] Nothing found in bigpool\n");

	return true;
}

PSYSTEM_BIGPOOL_INFORMATION pool::get_big_pool_info()
{
	ULONG size = 4 * 1024 * 1024;
	void* buffer = ExAllocatePool(NonPagedPool, size);
	if (!buffer)
		return nullptr;

	PSYSTEM_BIGPOOL_INFORMATION info = reinterpret_cast<PSYSTEM_BIGPOOL_INFORMATION>(buffer);
	if (!NT_SUCCESS(ZwQuerySystemInformation(0x42, buffer, size, &size))) /* SystemBigPoolInformation */
	{
		ExFreePool(buffer);
		return nullptr;
	}

	return info;
}
