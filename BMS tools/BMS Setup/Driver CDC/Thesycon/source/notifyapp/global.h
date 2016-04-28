/************************************************************************
 *
 *  Module:       global.h
 *  Description:
 *     APP global includes, constants, declarations, etc.
 *
 *  Author(s):   
 *    Udo Eberhardt
 *                
 *  Companies:
 *    Thesycon GmbH, Germany      http://www.thesycon.de
 *                
 ************************************************************************/

#ifndef __global_h__
#define __global_h__

// define the Windows versions supported by the application
#define _WIN32_WINNT 0x0500     //Windows 2000 or later
//#define _WIN32_WINNT 0x0501     //Windows XP or later
//#define _WIN32_WINNT 0x0600     //Windows Vista or later

// exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// windows API
#include <windows.h>
// setup API
#include <setupapi.h>
// defines for WM_DEVICECHANGE messages of the notificator 
#include <dbt.h>

#include <stdio.h>
#include <tchar.h>

// the class PnP notificator
#include "PnPNotificator.h"
// the class port information
#include "PortInfo.h"
// the project related notificator
#include "portNotificator.h"


// version defs
#include "version.h"

// Note: This GUID must match the entry in the INF file
// HKR,,DriverUserInterfaceGuid,%REG_SZ%,"{40994DFA-45A8-4da7-8B58-ACC2D7CEA825}"
// this entry must be modified during customization, please create a new GUID and enter it 
// in the INF file and use it in your application
// this GUID should be used only for this example, see documentation for details

// {40994DFA-45A8-4da7-8B58-ACC2D7CEA825}
#define VENDOR_IID { 0x40994dfa, 0x45a8, 0x4da7, { 0x8b, 0x58, 0xac, 0xc2, 0xd7, 0xce, 0xa8, 0x25 }}


// a helper function in this program
DWORD
DisplayDevices(char* DevicePath = NULL);

#endif  // __global_h__

/*************************** EOF **************************************/
