@echo off
setlocal

@rem *** path to bin dir where .sys files are located
set BINDIR=.\bin

@rem *** path to INF dir where .inf files are located
set INFDIR=.

@rem *** subdir for debug driver package
set TGTDIR_DBG=dbg

@rem *** subdir for release driver package
set TGTDIR_REL=rel

@rem prefixes for signed and unsigned packages
set TGTDIR_PARENT_SIGNED=driver_package
set TGTDIR_PARENT_UNSIGNED=driver_package_notsigned

@rem *** the Thesycon preprocessor
set PP=.\tools\TPP.exe

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

@rem load the version setting
call .\setver.cmd

@rem load the customer settings
call .\set_vars.cmd

@rem load the certificate settings
call .\signing\set_vendor_certificate.cmd 

@rem remove the quotes, if any
@set DRIVER_NAME_BASE=%DRIVER_NAME_BASE:"=%

call .\signing\set_sign_tools.cmd 

@rem check if SignTools package is available
set UNSIGNED=
if not "%SIGNTOOLS%"=="" goto cont1
set UNSIGNED=1
:cont1

@rem check if Vendor Certificate is set
if not "%VENDOR_CERTIFICATE%"=="" goto cont2
echo.
echo Warning: VENDOR_CERTIFICATE environment variable is not set! Please   
echo install a vendor certificate and set the certificate name in 
echo .\signing\set_vendor_certificate.cmd. 
echo See reference manual for more details. 
echo.
pause
set UNSIGNED=1
:cont2

if "%UNSIGNED%"=="" goto cont3
@echo.
@echo The prerequisites to create a signed driver package are not fulfilled!
@echo When you continue the srcipt generates a unsigned package. 
@echo The unsigned package can be used on 32 bit systems and
@echo on Windows Vista 64 bit and Windows 7 64 bit, only. 
@echo It cannot be installed on Windows 8 64 bit!
@echo During the driver installation the system shows a warning!
@echo.
pause
set TGTDIR_DBG=%TGTDIR_PARENT_UNSIGNED%\%TGTDIR_DBG%
set TGTDIR_REL=%TGTDIR_PARENT_UNSIGNED%\%TGTDIR_REL%
goto cont4
:cont3
set TGTDIR_DBG=%TGTDIR_PARENT_SIGNED%\%TGTDIR_DBG%
set TGTDIR_REL=%TGTDIR_PARENT_SIGNED%\%TGTDIR_REL%
:cont4

@echo on

@rem delete target dirs
rmdir /S /Q %TGTDIR_PARENT_SIGNED% >nul 2>nul
rmdir /S /Q %TGTDIR_PARENT_UNSIGNED% >nul 2>nul
@rem create target dirs
@if EXIST .\bin\debug (
  mkdir %TGTDIR_DBG% >nul 2>nul
  @if %errorlevel% NEQ 0 pause
)
mkdir %TGTDIR_REL% >nul 2>nul
@if %errorlevel% NEQ 0 pause

@rem copy .inf files
@if EXIST .\bin\debug (
	call :DOPP %INFDIR%\cdcacmpo.inf.src %TGTDIR_DBG%\%DRIVER_NAME_BASE%.inf
)
call :DOPP %INFDIR%\cdcacmpo.inf.src %TGTDIR_REL%\%DRIVER_NAME_BASE%.inf

@rem copy .sys files
@if EXIST .\bin\debug (
	@call :CopySys %TGTDIR_DBG% debug
)

@if "%UNSIGNED%"=="" goto signed
@call :CopySys %TGTDIR_REL% signed
@goto end

:signed
@call :CopySys %TGTDIR_REL% unsigned

@echo on
@rem sign .sys files
set FILE_LIST=%TGTDIR_REL%\*.sys
@if EXIST .\bin\debug set FILE_LIST=%FILE_LIST% %TGTDIR_DBG%\*.sys

cmd /c .\signing\vendor_sign_files.cmd %FILE_LIST%

@rem create .cat files
@if EXIST .\bin\debug (
	%SIGNTOOLS%\bin\inf2cat.exe /driver:%TGTDIR_DBG%  /os:%OSLIST_x86%,%OSLIST_x64%
	@if %errorlevel% NEQ 0 pause
)

%SIGNTOOLS%\bin\inf2cat.exe /driver:%TGTDIR_REL%  /os:%OSLIST_x86%,%OSLIST_x64%
@if %errorlevel% NEQ 0 pause

@rem sign .cat files
set FILE_LIST=%TGTDIR_REL%\*.cat
@if EXIST .\bin\debug set FILE_LIST=%FILE_LIST% %TGTDIR_DBG%\*.cat

cmd /c .\signing\vendor_sign_files.cmd %FILE_LIST%

@echo off
goto end
rem *************** end of 'main'



rem *************** Procedures begin here ****************************

rem *** DOPP  %1=In-File [%2=Out-File]
:DOPP
@rem if no Out-File is given, use In-File without .src extension
@set OUTF=%2
@if "%OUTF%" == "" set OUTF=%~dpn1
@attrib -r %OUTF% >nul 2>nul
@echo on
%PP% %1 -o %OUTF% -f  %TPP_VERDEF_ARGS% %TPP_DEFS%
@if %errorlevel% NEQ 0 pause
@echo off
@attrib +r %OUTF% >nul 2>nul
@exit /b 0


rem *** CopySys  %1=TargetDir %2=source
:CopySys
@echo on
copy %BINDIR%\%2\cdcacmpo.sys      %1\%DRIVER_NAME_BASE%.sys
@if %errorlevel% NEQ 0 pause
copy %BINDIR%\%2\cdcacmpo_x64.sys  %1\%DRIVER_NAME_BASE%_x64.sys
@if %errorlevel% NEQ 0 pause
@echo off
@exit /b 0
rem ***


rem *************** Procedures end here ****************************


:end
endlocal
