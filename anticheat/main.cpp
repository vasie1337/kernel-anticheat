#include "include.h"

void DriverUnload(PDRIVER_OBJECT drv_obj)
{
	
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT drv_obj, PUNICODE_STRING reg)
{
	drv_obj->DriverUnload = DriverUnload;


	return STATUS_SUCCESS;
}