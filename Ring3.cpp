// Ring3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "Ring3.h"



Io_Control g_Ioincontrol = {0};
Io_Control g_Iooutcontrol = { 0 };
HANDLE g_hFile;
HANDLE hDevice;
int num = 1;
std::map<HANDLE,int>g_Mres;
int main()
{
	BOOLEAN bRet = FALSE;
	 hDevice = CreateFile(SYM_NAME,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device!");
		return -1;
	}
	else
	{
		printf("open device Successfully\n");
	}

	//在这写参数wszFilePath DesiredAccess FileAttributes ShareAccess CreateDisposition CreateOptions
	MyCreateFile(L"\\??\\C:\\MyCreateFile.txt",
			(0x80000000L) | (0x40000000L),
			(0x00000080),
			(0x00000001),
			(0x00000003),
			(0x00000020) | (0x00000800) | (0x00000040),
			NULL);

	////////////////////////do something//////////////////////////////////

	//Sleep(500);

//////////////////////////////////////////////////////////////////////////
	//参数是索引号
	MyCloseFile(1);

	CloseHandle(hDevice);
	system("pause");
}


void MyCreateFile(WCHAR* wszFilePath,
	ULONG DesiredAccess,
	ULONG FileAttributes,
	ULONG  ShareAccess,
	ULONG  CreateDisposition,
	ULONG  CreateOptions,
	PHANDLE phFile)
{
	DWORD dwsize = sizeof(Io_Control);
	DWORD dwOutput;
	Io_Control m_Iocontrol = { wszFilePath,
					DesiredAccess,
					FileAttributes,
					ShareAccess,
					CreateDisposition,
					CreateOptions,
					phFile };
	g_Ioincontrol = m_Iocontrol;
	BOOLEAN bRet = FALSE;
	//发收
	bRet = DeviceIoControl(hDevice,
		IOCTL_TESTWRITEREAD,
		&g_Ioincontrol,
		dwsize,
		&g_Iooutcontrol,
		dwsize,
		&dwOutput, 
		NULL);
	printf("Output buffer:%d bytes\n", dwOutput);
	if (!bRet)
	{
		printf("接收or发送不成功！\n");
	}
	g_hFile = g_Iooutcontrol.hFile;
	g_Mres[g_hFile] = num++;
	printf("该句柄号为：%u\n 索引为：%d\n", g_hFile, g_Mres[g_hFile]);
}

VOID MyCloseFile(int num)
{
	DWORD dwOutput;
	BOOLEAN bRet = FALSE;
	HANDLE hRes;
	for (auto i : g_Mres)
	{
		if (num == i.second)
		{
			printf("找到句柄号!\n");
			hRes =i.first;
			bRet = DeviceIoControl(hDevice,
				IOCTL_CLOSEHANDLE,
				&hRes,
				sizeof(HANDLE),
				NULL,
				0,
				&dwOutput,
				NULL);

			if (!bRet)
			{
				printf("发送句柄号不成功！\n");
			}

			//if (0==CloseHandle(i.first))
			//{
			//	printf("关闭句柄失败!\n!");
			//	return FALSE;
			//}else printf("关闭句柄成功!\n");
			break;
		}
	}
}

//int Encrypt_Base64_Blowfish(char *input, int inputlen, char **output, int *outputlen, uint8_t g_szBlowfishKey)
//{
//	int nRet = 0;
//	uint8_t* encodeBase64Buffer = NULL;
//	char* cacheoutput = NULL;
//	do {
//		if (input == NULL || output == NULL || outputlen == NULL) {
//			break;
//		}
//		if (inputlen <= 0) {
//			break;
//		}
//		if (g_driverCodec.Initialize((const uint8_t*)g_szBlowfishKey, 32) == false)
//			break;
//		int nBufferLen = g_driverCodec.GetOutputLength(inputlen);
//		if (nBufferLen <= 0) {
//			break;
//		}
//		int encodeBase64BufferLen = modp_b64_encode_len(nBufferLen);
//		if (encodeBase64BufferLen <= 0) {
//			break;
//		}
//		encodeBase64Buffer = static_cast<uint8_t*>(malloc(encodeBase64BufferLen));
//		if (encodeBase64Buffer == NULL) {
//			break;
//		}
//		memset(encodeBase64Buffer, 0, encodeBase64BufferLen);
//		cacheoutput = static_cast<char*>(malloc(encodeBase64BufferLen));
//		if (cacheoutput == NULL) {
//			free(encodeBase64Buffer);
//			encodeBase64Buffer = NULL;
//			break;
//		}
//		memset(cacheoutput, 0, nBufferLen + 1);
//		if (g_driverCodec.Encode((uint8_t*)input,  // NOLINT // encrypt lib
//			encodeBase64Buffer, inputlen)) {
//			int output_size = CXtlBase64::encode(
//				cacheoutput, (char*)encodeBase64Buffer, nBufferLen);  // NOLINT // encrypt lib
//			if (output_size < 0) {
//				free(encodeBase64Buffer);
//				encodeBase64Buffer = NULL;
//				free(cacheoutput);
//				cacheoutput = NULL;
//				break;
//			}
//			*output = static_cast<char*>(cacheoutput);
//			*outputlen = output_size;
//			nRet = 1;
//			free(encodeBase64Buffer);
//			encodeBase64Buffer = NULL;
//		}
//	} while (0);
//	return nRet;
//}

