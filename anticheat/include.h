#pragma once

#include <intrin.h>
#include <ntifs.h>
#include <ntstrsafe.h>
#include <windef.h>

#define printf(text, ...) DbgPrintEx(DPFLTR_IHVBUS_ID, 0, text, ##__VA_ARGS__)

#include "kernel.h"

#include "utils/utils.h"

#include "methods/drivers/drivers.h"
#include "methods/handles/handles.h"
#include "methods/hypervisor/hv.h"
#include "methods/memory/pool.h"
#include "methods/other/serial.h"
#include "methods/other/traces.h"
#include "methods/threads/nmi.h"
#include "methods/threads/threads.h"

