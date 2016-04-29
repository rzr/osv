Thesycon CDC ACM device driver for Windows
==========================================

The driver creates a serial port for a device with one of the 
following interfaces:
a) Communication Device Class (CDC) Abstract Control Model (ACM)
b) two bulk pipes without status signals
c) special mode with two bulk pipes and status signals

DEMO Limitations (applicable for demo versions, only):
======================================================
The Demo version runs 4 hours after the device is connected the first 
time. After this point the read, write and open operation fails.
To re-enable the driver the PC must be restarted. This behavior is 
not part of a release version.


Package Content
===============
.\doc
  - documentation and license 

.\CustomPackageBuilder 
  - scripts and driver to create a customized driver package

.\source
  - source code example for device notifications and additional API's

.\app
  - the PnP notification application 


Driver Package Creation
=======================
The driver package must be customized before it can be installed. 
Please read the customization chapter in the reference manual for 
details.

Installation
============
Use the PnP Installer from Thesycon to create installation package
for your customers.
For details refer to the reference manual.


More Information
================
The reference manual contains more information related to:
- signing of drivers 
- configuration parameters
- operational modes
- enhanced error recovery 
- debugging of the driver

If there problems with the driver read the reference manual chapter 
Debug Support.


www.thesycon.com
info @thescon.de


