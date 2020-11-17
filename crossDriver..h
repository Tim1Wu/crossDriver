#ifndef _DRIVER_H_
#define _DRIVER_H_
#include<ntddk.h>

#define DEV_NAME L"\\Device\\IOCONTROL_LOG_DEV_NAME"
#define SYM_NAME L"\\DosDevices\\IOCONTROL_LOG_SYM_NAME"
#define IOCTL_TESTWRITEREAD CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_CLOSEHANDLE CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_WRITE_ACCESS)
typedef struct _Io_Control
{
	WCHAR* wszFilePath;
	ULONG DesiredAccess;
	ULONG FileAttributes;
	ULONG  ShareAccess;
	ULONG  CreateDisposition;
	ULONG  CreateOptions;
	HANDLE hFile;
}Io_Control,*PIo_Control;

typedef struct _DEVICE_EXTENSION
{
	LIST_ENTRY ListHeader;

}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

VOID DriverUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS DriverDefaultHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DriverControlHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject);
#endif
