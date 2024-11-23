#include "include.h"

void DriverUnload(PDRIVER_OBJECT drv_obj)
{
	UNREFERENCED_PARAMETER(drv_obj);
	printf("Driver Unloaded\n");
	return;
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT drv_obj, PUNICODE_STRING reg)
{
	drv_obj->DriverUnload = DriverUnload;

	printf("Driver Loaded\n");

	g_hv.check_vmread();

	return STATUS_SUCCESS;
}