#include "handles.h"

bool handles::check_physical_mem_handles()
{
	void* object = nullptr;
	HANDLE phys_mem_handle = nullptr;
	UNICODE_STRING phys_mem_str = RTL_CONSTANT_STRING(L"\\Device\\PhysicalMemory");
	OBJECT_ATTRIBUTES oa_attributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&phys_mem_str, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE);

	NTSTATUS status = ZwOpenSection(&phys_mem_handle, SECTION_ALL_ACCESS, &oa_attributes);
	if (!NT_SUCCESS(status))
		return false;

	status = ObReferenceObjectByHandle(phys_mem_handle, 1, nullptr, KernelMode, &object, nullptr);
	if (!NT_SUCCESS(status))
	{
		ZwClose(phys_mem_handle);
		return false;
	}

	ZwClose(phys_mem_handle);

	PSYSTEM_HANDLE_INFORMATION handles = get_handle_list();
	if (!handles)
	{
		ObDereferenceObject(object);
		return false;
	}

	for (ULONG i = 0; i < handles->uCount; i++)
	{
		if (handles->Handles[i].uIdProcess == 4)
			continue;

		if (handles->Handles[i].pObject == object)
		{
			if (!ObIsKernelHandle(reinterpret_cast<HANDLE>(handles->Handles[i].Handle)))
			{
				printf("Usermode PhysicalMemory handle detected, pid = %d, access = 0x%x.\n",
					handles->Handles[i].uIdProcess, 
					handles->Handles[i].GrantedAccess
				);
			}
		}
	}

	ObDereferenceObject(object);
	ExFreePool(handles);

	return true;
}

PSYSTEM_HANDLE_INFORMATION handles::get_handle_list()
{
	ULONG size = 8 * 1024 * 1024;

	PSYSTEM_HANDLE_INFORMATION list = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(ExAllocatePool(NonPagedPool, size));
	if (!list)
		return nullptr;

	NTSTATUS status = ZwQuerySystemInformation(SystemHandleInformation, list, size, 0);
	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		ExFreePool(list);
		return nullptr;
	}

	return list;
}