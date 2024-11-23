#include "serial.h"

void serial::print()
{
	PrintBootUUID();
	PrintDiskUUID();
	PrintMachineGuid();
	PrintVolumeSerialNumber();
	PrintCpuId();
	PrintBiosSerialNumber();
	PrintBaseboardSerialNumber();
	PrintSystemBiosVersion();
	PrintSystemSerialNumber();
	PrintSystemUuid();
}

void serial::PrintBootUUID()
{
	ULONG size = 8 * 1024 * 1024;

	PSYSTEM_BOOT_ENVIRONMENT_INFORMATION boot_info = reinterpret_cast<PSYSTEM_BOOT_ENVIRONMENT_INFORMATION>(ExAllocatePool(NonPagedPool, size));
	if (!boot_info)
		return;

	if (ZwQuerySystemInformation(0x5a, boot_info, size, nullptr) != STATUS_SUCCESS) /* SystemBootEnvironmentInformation */
	{
		ExFreePool(boot_info);
		return;
	}

	printf("[SERIAL] BootUUID : %08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X\n",
		boot_info->BootIdentifier.Data1, 
		boot_info->BootIdentifier.Data2, 
		boot_info->BootIdentifier.Data3, 
		boot_info->BootIdentifier.Data4[0], 
		boot_info->BootIdentifier.Data4[1], 
		boot_info->BootIdentifier.Data4[2], 
		boot_info->BootIdentifier.Data4[3],
		boot_info->BootIdentifier.Data4[4],
		boot_info->BootIdentifier.Data4[5],
		boot_info->BootIdentifier.Data4[6],
		boot_info->BootIdentifier.Data4[7]
	);
}

void serial::PrintDiskUUID()
{
}

void serial::PrintMachineGuid()
{
}

void serial::PrintVolumeSerialNumber()
{
}

void serial::PrintCpuId()
{
}

void serial::PrintBiosSerialNumber()
{
}

void serial::PrintBaseboardSerialNumber()
{
}

void serial::PrintSystemBiosVersion()
{
}

void serial::PrintSystemSerialNumber()
{
}

void serial::PrintSystemUuid()
{
}
