/************************************************************************
 *
 *  Module:       portNotificator.cpp
 *  Description:
 *     a special notificator class called if a virtual com port device is added or removed
 *
 *  Author(s):   
 *    Guenter Hildebrandt
 *                
 *  Companies:
 *    Thesycon GmbH, Germany      http://www.thesycon.de
 *                
 ************************************************************************/
#include "global.h"


//virtual 
void 
PortNotificator::HandlePnPMessage(
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		)
{
	DEV_BROADCAST_HDR *Header = (DEV_BROADCAST_HDR*)lParam;
	DEV_BROADCAST_DEVICEINTERFACE* Interface = (DEV_BROADCAST_DEVICEINTERFACE*)Header;

	// check the uMsg contains WM_DEVICECHANGE, this should be always the case
	if (uMsg == WM_DEVICECHANGE ) {
		// switch over the event type
		// we are looking only for arrival and remove complete
		switch(wParam) {
			case DBT_DEVICEARRIVAL:
				if (Header->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
					printf("\nDevice connected, new COM port available.\n");
					// the Interface->dbcc_name is aunique identifier for the device 
					// it can be used to open the COM port with CreateFile or to get
					// the COM port number with PortInfo::GetPortNameByDevicePath()
					::DisplayDevices(Interface->dbcc_name);
					// here you can create a process to work with the new virtual device
				}
				break;
			case DBT_DEVICEREMOVECOMPLETE:
				if (Header->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
					printf("\nDevice removed, COM port disappears.\n");
					::DisplayDevices();
				}
				break;
			default:;
				// do nothing
		}
	}
} // HandlePnPMessage

/*************************** EOF **************************************/
