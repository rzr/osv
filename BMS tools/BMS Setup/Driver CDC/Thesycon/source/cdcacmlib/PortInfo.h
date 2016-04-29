/************************************************************************
*
*  Module:       PortInfo.h
*  Long name:    Port Info class definition
*  Description:  implements a wrapper to get serial COM port information
*
*  Runtime Env.: Win32
*  Author(s):    Guenter Hildebrandt
*  Company:      Thesycon GmbH, Ilmenau
************************************************************************/

#ifndef _PortInfo_h_
#define _PortInfo_h_

/*doc CPortInfo
\begin{class}{}{ class}{CPortInfo}{
The \temph{CPortInfo} class defines an interface that is used to get
information about available virtual COM ports created by the CDC/ACM driver.
The COM port name can be requested either with an index or with the 
device path received by the PnPNotificator class.

}
*/
#define MAX_PORT_NAME_SIZE 10
#define MAX_SERIAL_NUMBER_SIZE 128

typedef struct tagPortInfoData{
	DWORD Index;
	char PortName[MAX_PORT_NAME_SIZE];
	char SerialNumber[MAX_SERIAL_NUMBER_SIZE];
} PortInfoData;
/*doc struct
\begin{structtype}{}{}{struct}{tagPortInfoData}{PortInfoData}{}{
This structure contains information about the COM port.
}
\member{DWORD}{Index}{
Contains the index of the current device list. The index may change if the function \temph{EnumeratePorts}
is called.
}
\member{char}{PortName[MAX_PORT_NAME_SIZE]}{
Returns the port name, e.g. COM3. The string is zero terminated.
}
\member{char}{SerialNumber[MAX_SERIAL_NUMBER_SIZE]}{
Returns the serial number of the device. This can be the serial number that is reported from the device 
as a string descriptor with the index iSerialNumber. If the device does not support such a serial number 
Windows creates a unique number for the device. The Windows created number changes if the device is 
connected to a different USB port. The string is zero terminated.
}
\comments{
This structure is used to get information about a COM port. 
}
\seealso{ 
\sa{CPortInfo::GetPortInfo}\\
\sa{CPortInfo::GetPortInfoByDevicePath}

}
\end{structtype}
\pagebreak
*/

class CPortInfo
{
public:
	CPortInfo();
	~CPortInfo();

	//
	// enumerate all active COM ports registered with a special driver interface
	// return 0 on success or a Windows error code
	DWORD 
	EnumeratePorts(const GUID* DriverInterface);
	/*doc CPortInfo
	\begin{function}{}{}{DWORD}{EnumeratePorts}{}{
	This function creates a internal list of all available virtual COM ports created by the CDC/ACM driver.
	}
	\param{const GUID*}{DriverInterface}{}{
	The \temph{DriverInterface} parameter contains the \temph{DriverUserInterfaceGuid} that is defined 
	in the INF file of the CDCACM driver with the line
	HKR,,DriverUserInterfaceGuid,\%REG_SZ\%,"{40994DFA-45A8-4da7-8B58-ACC2D7CEA825}".

	This GUID must be modified during the customization of the driver. This makes sure that 
	the notificator finds exactly the requested devices. Please create a new GUID and replace it
	in the INF file and in your application.
	}
	\retval{
	The function returns 0 on success or a windows error message.
	}
	\comments{
	The internal list of devices is created each time this function is called. The internal list contains 
	the devices that are available while this function is called. The list is destroyed 
	if the destructor of the class is called. If you want to find the COM port name of a removed 
	device instance, you need a list that was created while the device was connected. This function
	must be called before all other functions of this class.
	}
	\seealso{
	\sa{CPortInfo::GetPortCount}\\
	\sa{CPortInfo::GetPortInfo}\\
	\sa{CPortInfo::GetPortInfoByDevicePath}
	}
	\end{function}
	*/

	//
	// get port count returns the number of available ports
	//
	DWORD 
	GetPortCount();
	/*doc
	\begin{function}{}{}{DWORD}{GetPortCount}{}{
	This function returns the number of available virtual COM ports.
	}
	\retval{
	The function returns the number of available virtual COM ports.
	}
	\comments{
	It returns 0 if the function \temph{EnumeratePorts} was not called successful or if
	no devices are connected.
	}
	\seealso{
	\sa{CPortInfo::EnumeratePorts}\\
	}
	\end{function}
	*/

	//
	// returns the com port name
	DWORD
	GetPortInfo(
		DWORD Index,				// IN zero based index of the port list
		PortInfoData* PortInfo			// OUT caller provided data structure that receives the information
		);
	/*doc
	\begin{function}{}{}{DWORD}{GetPortInfo}{}{
	This function returns the COM port information to an index.
	}
	\param{DWORD}{Index}{}{
	The \temph{Index} parameter contains the zero based index to the internal device list 
	created with \temph{EnumeratePorts}. The function \temph{GetPortCount} can be used 
	to determine the number of available ports. Or this function can be called until the 
	Windows error \temph{ERROR_NO_MORE_ITEMS} is returned.
	}
	\param{PortInfoData*}{PortInfo}{}{
	The \temph{PortInfo} parameter contains a user provided data structure that receives the 
	COM port information. 
	}
	\retval{
	The function returns 0 on success or a windows error message.
	}
	\comments{
	The index is related to the device list. As long as the function \temph{EnumeratePorts} is not called
	the index returns always the same string. The relation between the index and COM port numbers 
	can be assigned in a different way if the function \temph{EnumeratePorts} is called.
	}
	\seealso{
	\sa{CPortInfo::EnumeratePorts}\\
	\sa{CPortInfo::GetPortCount}\\
	\sa{CPortInfo::GetPortInfoByDevicePath}\\
	\sa{PortInfoData}
	}
	\end{function}
	*/



	DWORD
	GetPortInfoByDevicePath(
		char* Path,					// path
		PortInfoData* PortInfo			// OUT caller provided data structure that receives the information
		);
	/*doc
	\begin{function}{}{}{DWORD}{GetPortInfoByDevicePath}{}{
	This function returns the COM port name to a device path.
	}
	\param{char*}{Path}{}{
	The \temph{Path} parameter contains the device path as a zero terminated string. This string  
	can be obtained in the PnP handler.
	}
	\param{PortInfoData*}{PortInfo}{}{
	The \temph{PortInfo} parameter contains a user provided data structure that receives the 
	COM port information. 
	}
	\retval{
	The function returns 0 on success or a windows error message.
	}
	\comments{
	The function fails with the Windows error code \temph{ERROR_NO_MORE_ITEMS} if the 
	device path is not in the list of the devices. 
	}
	\seealso{
	\sa{CPortInfo::EnumeratePorts}\\
	\sa{CPortInfo::GetPortCount}\\
	\sa{CPortInfo::GetPortInfo}\\
	\sa{PortInfoData}
	}
	\end{function}
	*/
protected:

	void
	ReleasePortList();

	HDEVINFO mDeviceList;
	GUID	mDriverInterface;

}; // CPortInfo
/*doc
\end{class}
*/

#endif // _PortInfo_h_

/*************************** EOF **************************************/