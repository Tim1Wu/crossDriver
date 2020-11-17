
#include "FileManage.h"
#include"Driver.h"



extern HANDLE g_hFile;
PDEVICE_OBJECT g_pDevObj = NULL;
PUNICODE_STRING g_pulSrcPath;
PUNICODE_STRING g_pulDestPath;

UNICODE_STRING g_ustrFilePath;

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	DbgPrint("Enter DriverEntry!\n");

	pDriverObject->DriverUnload = DriverUnload;

	for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = DriverDefaultHandle;
	}

	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverControlHandle;

	CreateDevice(pDriverObject);
	return STATUS_SUCCESS;
}

NTSTATUS DriverDefaultHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Enter DriverDefaultHandle\n");

	NTSTATUS status = STATUS_SUCCESS;

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("Leave DriverDefaultHandle\n");
	return status;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Enter DriverUnload\n");
	PDEVICE_OBJECT pDevObj = pDriverObject->DeviceObject;
	LARGE_INTEGER liDelay = { 0 };
	UNICODE_STRING ustrSymName;
	RtlInitUnicodeString(&ustrSymName, SYM_NAME);
	IoDeleteSymbolicLink(&ustrSymName);

	if (NULL == pDevObj) return;
	if (NULL == pDevObj->DeviceExtension) return;

	if (pDriverObject->DeviceObject)
	{
		IoDeleteDevice(pDriverObject->DeviceObject);
	}

	DbgPrint("Leave DriverUnload\n");
}

NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Enter CreateDevice\n");
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING ustrDevName, ustrSymName;
	

	RtlInitUnicodeString(&ustrDevName, DEV_NAME);
	RtlInitUnicodeString(&ustrSymName, SYM_NAME);

	// 创建通信设备
	status = IoCreateDevice(pDriverObject, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &g_pDevObj);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateDevice Error[0x%X]\n", status);
		return status;
	}

	// 创建符号连接
	status = IoCreateSymbolicLink(&ustrSymName, &ustrDevName);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateSymbolicLink Error[0x%X]\n", status);
		IoDeleteDevice(g_pDevObj);
		return status;
	}



	DbgPrint("Leave CreateDevice\n");
	return status;
}

NTSTATUS DriverControlHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Enter DriverControlHandle\n");
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackLocation = IoGetCurrentIrpStackLocation(pIrp);
	ULONG ulInputLength = pIoStackLocation->Parameters.DeviceIoControl.InputBufferLength;
	ULONG ulOutputLength = pIoStackLocation->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG ulControlCode = pIoStackLocation->Parameters.DeviceIoControl.IoControlCode;
	ULONG ulInfo = 0;
	PVOID pBuf = pIrp->AssociatedIrp.SystemBuffer;
	PIo_Control g_ioinputControl = { 0 };

	switch (ulControlCode)
	{
	case IOCTL_TESTWRITEREAD:
	{
		DbgPrint(" Enter IOCTL_TESTWRITEREAD!\n");
		g_ioinputControl = (PIo_Control)pBuf;
		
		RtlInitUnicodeString(&g_ustrFilePath,g_ioinputControl->wszFilePath);

		ACCESS_MASK  DesiredAccess = (ACCESS_MASK)g_ioinputControl->DesiredAccess;
		ULONG FileAttributes = g_ioinputControl->FileAttributes;
		ULONG ShareAccess = g_ioinputControl->ShareAccess;
		ULONG CreateDisposition = g_ioinputControl->CreateDisposition;
		ULONG CreateOptions = g_ioinputControl->CreateOptions;
		MyCreateFile(g_ustrFilePath,
			DesiredAccess,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions);

		PIo_Control g_iooutputControl = (PIo_Control)pBuf;
		g_iooutputControl->hFile = g_hFile;
		DbgPrint("句柄号%u", g_iooutputControl->hFile);
		ulInfo = ulOutputLength;
		break;
	}

	case IOCTL_CLOSEHANDLE:
	{
		DbgPrint(" Enter IOCTL_CLOSEHANDLE!\n");

		PHANDLE m_hFile = (PHANDLE)pBuf;
		DbgPrint("句柄号%u", *m_hFile);
		if (!NT_SUCCESS(ZwClose(*m_hFile)))
		{
			DbgPrint("删除句柄失败!\n");
		}

		break;
	}

	default:
		break;
	}
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulInfo;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("Enter DriverControlHandle\n");
	return status;
}

