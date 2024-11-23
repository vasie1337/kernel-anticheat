#pragma once
#include "../../include.h"

class handles
{
public:
	/// <summary>
	/// Check for physical memory handles in the handle list.
	/// </summary>
	/// <returns>
	/// Returns true if successful, false otherwise. Result is printed to the debug output.
	/// </returns>
	bool check_physical_mem_handles();

private:
	/// <summary>
	/// Get the handle list.
	/// </summary>
	/// <returns>
	/// Returns the handle list.
	/// </returns>
	PSYSTEM_HANDLE_INFORMATION get_handle_list();
};