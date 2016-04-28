; *********************************************************************
; *** the read buffer size grater than the FIFO size requires short ***
; *** packets from the device, see documentation for details!       ***
; *********************************************************************
HKR,,ReadBufferSize,  %REG_DWORD%, 4096
HKR,,WriteBufferSize, %REG_DWORD%, 4096
HKR,,UseLogicBuffer,  %REG_DWORD%, 0
HKR,,ReadBufferCount, %REG_DWORD%, 3
HKR,,WriteBufferCount,%REG_DWORD%, 3
HKR,,SendLineCoding,  %REG_DWORD%, 1
HKR,,SendLineState,   %REG_DWORD%, 1
HKR,,SendBreak,       %REG_DWORD%, 1

; 0 - CDCACM, 1 - Bulk Only, 2 - Bulk special, 3 - automatic
HKR,,OperationMode,%REG_DWORD%, 3

; set this to 0 to suppress the Clear Feature Endpoint Halt during Open and Wakeup
; this does not work on Windows 2000!
HKR,,ClearFeatureOnStart,%REG_DWORD%, 1
HKR,,VendorPipeReset,%REG_DWORD%, 0

; default serial line states reported in bulk only mode or 
; if the device never updates the status values
; use a or'ed value of: CTS=0x10, DSR=0x20, RI=0x40, DCD=0x80
HKR,,DefaultLineState,%REG_DWORD%, 0x10


HKR,,StaticDeviceObject,%REG_DWORD%,0
HKR,,ClearRtsDtrOnClose,%REG_DWORD%,0
; 0 - user timeout with STATUS_TIMEOUT, no bytes transferred
; 1 - emulate send with transfer time, STATUS_SUCCESS, all bytes transferred
HKR,,DisConWriteMode,%REG_DWORD%,0

; the default prefix for the device object
HKR,,DeviceObjectName,%REG_SZ%,"thcdcacm"

; set this to 1 to avoid any short packets in TX
HKR,,DoNotSendShortPackets,%REG_DWORD%,0

[_Common_AddReg]
; is a serial port
HKR,,PortSubClass,1,01
; property page, system default
HKR,,EnumPropPages32,,"MsPorts.dll,SerialPortPropPageProvider"


; If your device is a Windows CE system with bulk only communication 
; the following paramters my be used.
;HKR,,SendLineCoding,  %REG_DWORD%, 0
;HKR,,SendLineState,   %REG_DWORD%, 0
;HKR,,SendBreak,       %REG_DWORD%, 0
;HKR,,OperationMode,%REG_DWORD%,    1
;HKR,,DefaultLineState,%REG_DWORD%, 0x30
