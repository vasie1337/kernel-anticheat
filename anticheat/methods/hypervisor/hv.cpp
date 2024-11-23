#include "hv.h"

void hv::check_vmread()
{
	__try {
		__vmx_vmread(0, 0);
		DbgPrint("Hypervisor running\n");
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		DbgPrint("Hypervisor not running\n");
	}
}
