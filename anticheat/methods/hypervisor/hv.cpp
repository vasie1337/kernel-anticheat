#include "hv.h"

void hv::check_vmread()
{
	__try {
		__vmx_vmread(0, 0);
		printf("Hypervisor running\n");
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		printf("Hypervisor not running\n");
	}
}
