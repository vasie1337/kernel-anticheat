#pragma once
#include "../../include.h"

class threads
{
public:
	void scan_system_threads();

private:
	uintptr_t get_thread_start_address(PETHREAD thread);
};

inline threads g_threads;