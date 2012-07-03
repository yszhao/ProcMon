#include <ntddk.h>
NTSTATUS PsLookupProcessByProcessId(__in   HANDLE ProcessId,
									__out  PEPROCESS *Process);

static VOID LoadImageNotifyRoutine( PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);

VOID LoadImageNotifyRoutine( PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	HANDLE hProcess = NULL;
	NTSTATUS status;
	PEPROCESS peProcess;
	PWCHAR path;
	USHORT length;
	length = (FullImageName->Length) / 2;
	path = FullImageName->Buffer;
	if (path[length-1] == 'e' )
	{
		//DbgBreakPoint();
		status = PsLookupProcessByProcessId(ProcessId, &peProcess);
        if (status != STATUS_SUCCESS)
			DbgPrint(("Err PsLookupProcessByProcessId\n"));
		DbgPrint("Process ID: %d",ProcessId);
		DbgPrint("Full Name: %wZ",FullImageName);
		status = ZwTerminateProcess(peProcess, STATUS_SUCCESS);
		if(status)
			DbgPrint("ZwTerminateProcess error /n");
	}

}


VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject)
{
    PsRemoveLoadImageNotifyRoutine(  LoadImageNotifyRoutine );
    DbgPrint("Unload!\n");
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,  IN PUNICODE_STRING RegistryPath)
{
    PsSetLoadImageNotifyRoutine( LoadImageNotifyRoutine );
    DriverObject->DriverUnload = UnloadRoutine;
    DbgPrint("Driver loaded");
    
    return STATUS_SUCCESS; 

}