#include "hv.h"

void hv::check_vmread()
{
	__try {
		__vmx_vmread(0, 0);
		printf("[HV] Hypervisor running\n");
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		printf("[HV] Hypervisor not running\n");
	}
}
