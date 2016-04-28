/************************************************************************
*
*  Module:       PortInfo.cpp
*  Long name:    Port Info class definition
*  Description:  implements a wrapper to get serial COM port information
*
*  Runtime Env.: Win32
*  Author(s):    Guenter Hildebrandt
*  Company:      Thesycon GmbH, Ilmenau
************************************************************************/

#include "global.h"
#include "PortInfo.h"

CPortInfo::CPortInfo()
{
	mDeviceList = INVALID_HANDLE_VALUE;
	memset((void*)&mDriverInterface,0,sizeof(mDriverInterface));
}


CPortInfo::~CPortInfo()
{
	ReleasePortList();
}


DWORD 
CPortInfo::EnumeratePorts(
	const GUID* DriverInterface
	)
{
	ReleasePortList();
	// struct copy
	mDriverInterface = *DriverInterface;

	mDeviceList = SetupDiGetClassDevs(
		(GUID*)DriverInterface,                 // LPGUID ClassGuid, 
		NULL,                                 // PCTSTR Enumerator, 
		NULL,                                 // HWND hwndParent, 
		DIGCF_DEVICEINTERFACE | DIGCF_PRESENT // DWORD Flags
		);
	return ( (mDeviceList == INVALID_HANDLE_VALUE) ? GetLastError() : 0 );
}

DWORD 
CPortInfo::GetPortCount()
{
	if (mDeviceList == INVALID_HANDLE_VALUE) {
		return 0;
	}

	DWORD Count = 0;
	DWORD Status;
	BOOL succ;


	// enumerate the interface until ERROR_NO_MORE_ITEMS
	SP_DEVICE_INTERFACE_DATA DevData;
	for (Count = 0;; Count++ ) {
		ZeroMemory(&DevData,sizeof(DevData));
		DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		succ = SetupDiEnumDeviceInterfaces(mDeviceList, NULL, (GUID*)&mDriverInterface, Count, &DevData );
		if ( !succ ) {
			Status = GetLastError();
			if ( Status == ERROR_NO_MORE_ITEMS ) {
				break;
			}
		}
		if (Count > 10000) {
			// this should not happen, break a possible endless loop
			Count = 0;
			break;
		}
	}
	return Count;
}

DWORD
CPortInfo::GetPortInfo(
	DWORD Index,				// IN zero based index of the port list
	PortInfoData* PortInfo			// OUT caller provided data structure that receives the information
	)
{
	ZeroMemory(PortInfo,sizeof(PortInfoData));
	PortInfo->Index = Index;
	if (mDeviceList == INVALID_HANDLE_VALUE) {
		return ERROR_INVALID_FUNCTION;
	}
	DWORD Status;
	BOOL succ;

	// enumerate the interface until ERROR_NO_MORE_ITEMS
	SP_DEVINFO_DATA  DeviceInfoData;
	ZeroMemory(&DeviceInfoData,sizeof(DeviceInfoData));
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	succ = SetupDiEnumDeviceInfo(mDeviceList, Index, &DeviceInfoData );
	if ( !succ ) {
		return GetLastError();
	}
	HKEY key =
		SetupDiOpenDevRegKey(
												mDeviceList,    // IN HDEVINFO  DeviceInfoSet,
												&DeviceInfoData, // IN PSP_DEVINFO_DATA  DeviceInfoData,
												DICS_FLAG_GLOBAL, // IN DWORD  Scope,
												0,							// IN DWORD  HwProfile,
												DIREG_DEV,			// IN DWORD  KeyType,
												KEY_READ				// IN REGSAM  samDesired
												);
	if (key == INVALID_HANDLE_VALUE) {
		return GetLastError();
	}
	DWORD PortNameSize = MAX_PORT_NAME_SIZE - 1;
	Status = RegQueryValueEx(
		key, //  HKEY hkey,
		"PortName", // LPCTSTR lpValue,
		NULL, 
		NULL, //DWORD type,
		(LPBYTE)PortInfo->PortName, // PVOID pvData,
		&PortNameSize // LPDWORD pcbData
		);
	RegCloseKey(key);


	DWORD RequiredSize = 0;
	SetupDiGetDeviceInstanceId(
							mDeviceList, // HDEVINFO DeviceInfoSet,
							&DeviceInfoData, // PSP_DEVINFO_DATA DeviceInfoData,
							NULL, // PTSTR DeviceInstanceId,
							0, // DWORD DeviceInstanceIdSize,
							&RequiredSize // PDWORD RequiredSize
							);
	if (RequiredSize > 0) {
		char* DeviceInstanceId = new char[RequiredSize+1];
		if (DeviceInstanceId) {
			ZeroMemory(DeviceInstanceId,RequiredSize+1);
			BOOL succ =
			SetupDiGetDeviceInstanceId(
				mDeviceList, // HDEVINFO DeviceInfoSet,
				&DeviceInfoData, // PSP_DEVINFO_DATA DeviceInfoData,
				DeviceInstanceId, // PTSTR DeviceInstanceId,
				RequiredSize, // DWORD DeviceInstanceIdSize,
				&RequiredSize // PDWORD RequiredSize
				);
			if (succ) {
				char* start = strrchr(DeviceInstanceId,'\\');
				if (start) {
					strncpy(PortInfo->SerialNumber,start+1,MAX_SERIAL_NUMBER_SIZE-1);
				}
			} else {
				Status = GetLastError();
			}
			delete [] DeviceInstanceId;
		} else {
			Status = ERROR_NOT_ENOUGH_MEMORY;
		}
	} else {
		Status = GetLastError();
	}
	return Status;
}

DWORD
CPortInfo::GetPortInfoByDevicePath(
	 char* Path,					// path
	 PortInfoData* PortInfo			// OUT caller provided data structure that receives the information
	 )
{
	ZeroMemory(PortInfo,sizeof(PortInfoData));
	// check if we have a valid handle
	if (mDeviceList == INVALID_HANDLE_VALUE) {
		return ERROR_INVALID_FUNCTION;
	}
	DWORD Status;
	BOOL succ;
	DWORD index = 0;

	for (;;) {
		// enumerate the interface
		// get the device information for the given device number
		SP_DEVICE_INTERFACE_DATA DevData;
		ZeroMemory(&DevData,sizeof(DevData));
		DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		succ = SetupDiEnumDeviceInterfaces(mDeviceList, NULL, (GUID*)&mDriverInterface, index, &DevData );
		if ( !succ ) {
			Status = GetLastError();
			return Status;
		}

		// get length of the detailed information, allocate buffer
		DWORD ReqLen = 0;
		SetupDiGetDeviceInterfaceDetail(mDeviceList, &DevData, NULL, 0, &ReqLen, NULL);
		if ( ReqLen==0 ) {
			return GetLastError();
		}
		SP_DEVICE_INTERFACE_DETAIL_DATA* DevDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[ReqLen];
		if ( DevDetail==NULL ) {
			return 1;
		}

		// now get the  detailed device information
		ZeroMemory(DevDetail,ReqLen);
		DevDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		succ = SetupDiGetDeviceInterfaceDetail(mDeviceList, &DevData, DevDetail, ReqLen, &ReqLen, NULL);
		if ( !succ ) {
			Status = GetLastError();
			return Status;
		}
		// check if the device path is the same as passed in
		// if yes, get the port name

		if (0 == _stricmp(DevDetail->DevicePath,Path)) {
			delete [] DevDetail;
			return GetPortInfo(index,PortInfo);
		}
		delete [] DevDetail;
		// increment the index
		index++;
	} // for
} // GetPortNameByDevicePath


// ****************************************
// implementation
// ****************************************
void
CPortInfo::ReleasePortList()
{
	if (mDeviceList != INVALID_HANDLE_VALUE) {
		SetupDiDestroyDeviceInfoList(mDeviceList);
		mDeviceList = INVALID_HANDLE_VALUE;
	}
} // ReleasePortList


/*************************** EOF **************************************/
