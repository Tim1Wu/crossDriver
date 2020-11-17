#include <windows.h>
#include<winioctl.h>
#include <string.h>
#include <stdio.h>
#include<iostream>


#include<map>
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
}Io_Control, *PIo_Control;
#define SYM_NAME L"\\\\.\\IOCONTROL_LOG_SYM_NAME"

VOID MyCloseFile(int num);

void MyCreateFile(WCHAR* wszFilePath,
	ULONG DesiredAccess,
	ULONG FileAttributes,
	ULONG  ShareAccess,
	ULONG  CreateDisposition,
	ULONG  CreateOptions,
	PHANDLE phFile);

////º”Ω‚√‹
//static CBlowFish g_driverCodec;
//
//int Encrypt_Base64_Blowfish(char *input, int inputlen, char **output, int *outputlen, uint8_t g_szBlowfishKey);





