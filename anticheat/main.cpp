#include "include.h"

void DriverUnload(PDRIVER_OBJECT drv_obj)
{
	UNREFERENCED_PARAMETER(drv_obj);
	printf("[MAIN] Driver Unloaded\n");
	return;
}

EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT drv_obj, PUNICODE_STRING reg)
{
	drv_obj->DriverUnload = DriverUnload;

	printf("[MAIN] Driver Loaded\n");

	g_drivers.scan();
	g_handles.check_physical_mem_handles();
	g_hv.check_vmread();
	g_pool.scan_big_pool();
	g_serial.print();
	g_traces.check_pid_db_cache_table();
	g_threads.scan_system_threads();

	g_nmi.init();
	g_nmi.fire(4);
	g_nmi.scan();

	return STATUS_SUCCESS;
}