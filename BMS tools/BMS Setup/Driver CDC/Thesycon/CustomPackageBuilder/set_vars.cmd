@echo off

@rem *****************************************
@rem
@rem This sample configuration shows how the driver should be configured 
@rem
@rem *****************************************


@rem *** basic package configuration
set VENDOR_NAME="Thesycon"
set PRODUCT_NAME="CDC/ACM driver"

@rem Choose a unique name to be used for all driver files.
@rem !!! No spaces or special characters allowed in DRIVER_NAME_BASE !!!
set DRIVER_NAME_BASE="cdcacmpo"

@rem *** driver interfaces

@rem !!! Use guidgen.exe to create a fresh GUID !!!
@rem This GUID is used for the PnP notificator and when the driver is opened 
@rem by your application with a GUID instead of a COM port number.
@rem This example GUID works with the demonstration application notifyapp.exe.
@rem If not required leave it empty.
set DRIVER_INTERFACE_GUID="{40994DFA-45A8-4da7-8B58-ACC2D7CEA825}"



@rem List of device models supported by the driver package.
@rem Format for INF_VID_PID_x is: VID_xxxx&PID_xxxx, xxxx specifies VID/PID in hex format
@rem For Multi-Interface Devices use: VID_xxxx&PID_xxxx&MI_xx, xx specifies the interface number
@rem INF_VID_PID_x_DESCRIPTION text is shown in Device Manager. 
@rem Make sure these strings are quoted!

set INF_VID_PID_1="VID_152a&PID_0208"
set INF_VID_PID_1_DESCRIPTION="Loop test device"

set INF_VID_PID_2=
set INF_VID_PID_2_DESCRIPTION=

set INF_VID_PID_3=
set INF_VID_PID_3_DESCRIPTION=

set INF_VID_PID_4=
set INF_VID_PID_4_DESCRIPTION=


