#pragma once
#include "../../include.h"

class pool
{
public:
	bool scan_big_pool();
private:
	PSYSTEM_BIGPOOL_INFORMATION get_big_pool_info();
};