#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>

#pragma comment(lib, "Setupapi.lib")

#include "../types/mestring.h"
using namespace MorganaEngine::Base::Types;

DWORD devIterator = 0;
void DevWin32_ResetDevicesInterator()
{
	devIterator = 0;
}

String DevWin32_GetDeviceDescOfType( const GUID& guid )
{
	String dev_str;

	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;

	hDevInfo = SetupDiGetClassDevs((LPGUID) &guid, 0, 0, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		// Insert error handling here.
		return "";
	}

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
	{
		if(i < devIterator) continue;

		DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;

		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				// Change the buffer size.
				if (buffer) LocalFree(buffer);
				// Double the size to avoid problems on 
				// W2k MBCS systems per KB 888609. 
				buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);
			}
			else
			{
				// Insert error handling here.
				break;
			}
		}

		dev_str = String(buffer);
		//printf("Result:[%s]\n",buffer);

		if (buffer) LocalFree(buffer);

		devIterator++;
		break;
	}


	if ( GetLastError()!=NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS )
	{
		// Insert error handling here.
		return "";
	}

	//  Cleanup
	SetupDiDestroyDeviceInfoList(hDevInfo);

	return dev_str;
}

#endif