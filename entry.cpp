#include <ntdef.h>
#include <ntifs.h>
#include <ntimage.h>
#include <windef.h>

#include "definitions.h"
#include "encrypt.h"
#include "crt.h"
#include "utils.h"
#include "eac.h"

auto DriverEntry( ) -> NTSTATUS
{
	/*
	making this run when eac driver is loaded little paster
	*/

	const auto easy_anti_cheat = utils::get_kernel_module( e( "EasyAntiCheat.sys" ) );

	if ( !easy_anti_cheat.base || !easy_anti_cheat.size )
	{
		DbgPrintEx( 0, 0, e( "EasyAntiCheat.sys is not loaded." ) );
		return STATUS_UNSUCCESSFUL;
	}

	DbgPrintEx( 0, 0,e(  "found EasyAntiCheat.sys: 0x%llx with size of 0x%llx" ), easy_anti_cheat.base, easy_anti_cheat.size );
	
	if ( eac::hook_allocation( easy_anti_cheat.base, easy_anti_cheat.size ) )
	{
		DbgPrintEx( 0, 0, e( "successfully hooked EasyAntiCheat's allocation" ) );
	}
	else
	{
		DbgPrintEx( 0, 0, e( "failed hooking EasyAntiCheat's allocation" ) );
	}

	return STATUS_SUCCESS;
}