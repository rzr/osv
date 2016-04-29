@echo off
setlocal

if "%1"=="" goto error
if not exist %1 goto error2

@rem *** NOTE: We should always use the full list of OS.

@rem *** OS list for x86
set OSLIST_x86=2000,XP_X86,Server2003_X86,Vista_X86,7_X86,Server2008_X86,8_X86
@rem *** OS list for x64
set OSLIST_x64=XP_X64,Server2003_X64,Vista_X64,7_X64,Server2008_X64,Server2008R2_X64,8_X64,Server8_X64

@rem
@rem OS list contains a combination of the following values:
@rem    2000  XP_X86   Server2003_X86   Vista_X86  7_X86  Server2008_X86					  8_X86
@rem          XP_X64   Server2003_X64   Vista_X64  7_X64  Server2008_X64   Server2008R2_X64   8_X64  Server8_X64
@rem                   Server2003_IA64                    Server2008_IA64  Server2008R2_IA64
@rem                                                                                          8_ARM
@rem

@rem load the certificate settings
call .\signing\set_vendor_certificate.cmd 

call .\signing\set_sign_tools.cmd 

@rem check if SignTools package is available
if not "%SIGNTOOLS%"=="" goto cont1
goto end
:cont1

@echo on
@rem sign .sys files
cmd /c .\signing\vendor_sign_files.cmd %1\*.sys

@rem create .cat files
%SIGNTOOLS%\bin\inf2cat.exe /driver:%1  /os:%OSLIST_x86%,%OSLIST_x64%
@if %errorlevel% NEQ 0 pause

@rem sign .cat files
cmd /c .\signing\vendor_sign_files.cmd %1\*.cat

@echo off
goto end
rem *************** end of 'main'

:error
@echo ERROR:
@echo Please pass the folder with the customized INF and SYS files 
@echo as an command line argument to this script. Run this script 
@echo only, if you have made manual changes to the created driver 
@echo package. 
@echo.
@pause
@goto end

:error2
@echo ERROR:
@echo The folder %1
@echo does not exist. 
pause
goto end

:end
endlocal
