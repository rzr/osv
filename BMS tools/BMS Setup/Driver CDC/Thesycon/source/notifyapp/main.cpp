/************************************************************************
 *
 *  Module:       main.cpp
 *  Description:  a console program that prints PnP events of virtual COM ports 
 *                It demonstrate how the PnP events can be captured with the PnPNotificator class
 *                and it shows how the COM port names can be determined with the PortInfo class.
 *                A derived application may start an application to perform some data communication 
 *                on a connected device.
 *
 *  Runtime Env.: Win32 Console
 *  Author(s):    Guenter Hildebrandt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#include "global.h"

// a global instance of the vendor defines ID
// this ID must equal to the ID in the INF file
GUID VendorGuid = VENDOR_IID;

//
// this function displays all devices using the class PortInfo
// a device path is passed to this function the related COM port is determined
//
DWORD
DisplayDevices(
	char* DevicePath /*= NULL */
	)
{
	// create a class instance on the stack
	CPortInfo portInfo;
	DWORD Status;

	// enumerate all ports that available
	Status = portInfo.EnumeratePorts(&VendorGuid);
	if (Status) {
		printf("DisplayDevices:EnumeratePorts failed: %08x\n",Status);
		return Status;
	}

	// a string for the port name
	PortInfoData PortInfo;

	// check if we have a device path
	if (DevicePath != NULL) {
		// get the COM port name from the portInfo class
		Status = portInfo.GetPortInfoByDevicePath(DevicePath,&PortInfo);
		if (Status) {
			// on error print the error and continue
			printf("DisplayDevices:GetPortInfoByDevicePath failed: %08x\n",Status);
		} else {
			// print it on success
			printf("The new COM port has the name %s and serial number:%s\n",PortInfo.PortName,PortInfo.SerialNumber);
		}
	}

	// get the number of available ports
	DWORD Count = portInfo.GetPortCount();
	if (Count == 1) {
		printf("Currently %d virtual COM port is available.\n",Count);
	} else {
		printf("Currently %d virtual COM ports are available.\n",Count);
	}

	// run in a loop to get the com port names and print it
	for (DWORD i=0;i<Count;i++) {
		Status = portInfo.GetPortInfo(i,&PortInfo);
		if (Status) {
			// on error print it and leave the loop
			printf("DisplayDevices:GetPortInfo failed: %08x\n",Status);
			return Status;
		}
		// print the COM port name
		printf("Name of the virtual port is: %s Serial Number:%s\n",PortInfo.PortName,PortInfo.SerialNumber);
	}

	// return success
	return 0;
} // DisplayDevices


int main(int , const char* [])
{
	
	PortNotificator portNotify;

	// initialize the Notify handler, and start a background thread
	portNotify.Initialize(::GetModuleHandle(NULL),&portNotify);
	// enable the device notifications
	portNotify.EnableDeviceNotifications(VendorGuid);

	printf("Press Enter to exit.\n\n");
	// display all available virtual com ports
	DisplayDevices();
	
	// wait for enter
	getchar();

	// disable the notification callbacks
	portNotify.DisableDeviceNotifications(VendorGuid);
	// shutdown the background thread
	portNotify.Shutdown();
	return 0;
} //main


/******************************** EOF ***********************************/
