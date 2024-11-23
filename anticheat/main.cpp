#include "include.h"

void DriverUnload(PDRIVER_OBJECT drv_obj)
{
	UNREFERENCED_PARAMETER(drv_obj);
	DbgPrint("Driver Unloaded\n");
	return;
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT drv_obj, PUNICODE_STRING reg)
{
	drv_obj->DriverUnload = DriverUnload;

	DbgPrint("Driver Loaded\n");

	g_hv.check_vmread();

	return STATUS_SUCCESS;
}