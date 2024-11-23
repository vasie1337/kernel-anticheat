#pragma once
#include "../../include.h"

class serial
{
public:
	void PrintSerials();

private:
	void PrintBootUUID();
	void PrintDiskUUID();
	void PrintMachineGuid();
	void PrintVolumeSerialNumber();
	void PrintCpuId();
	void PrintBiosSerialNumber();
	void PrintBaseboardSerialNumber();
	void PrintSystemBiosVersion();
	void PrintSystemSerialNumber();
	void PrintSystemUuid();
};

inline serial g_serial;