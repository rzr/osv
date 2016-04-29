@rem *
@rem * Digitally sign a file using embedded signatures.
@rem * This works for .cat files and executable files in PE format: .sys, .dll, .exe, .ocx, etc.
@rem *

@echo off
setlocal

set RETCODE=0

if "%1"=="" goto help


@rem set variables
call "%~dp0set_sign_tools.cmd"
if not "%errorlevel%"=="0" goto end

call "%~dp0set_vendor_certificate.cmd"

@rem check strings
if "%VENDOR_CERTIFICATE%"=="" (
  echo ERROR: VENDOR_CERTIFICATE is not set. Edit set_vendor_certificate.cmd or your machine's environment.
  set RETCODE=-1
  pause
  goto end
)
if "%CROSS_CERTIFICATE%"=="" (
  echo ERROR: CROSS_CERTIFICATE is not set. Edit set_vendor_certificate.cmd or your machine's environment.
  set RETCODE=-1
  pause
  goto end
)
if "%SIGNTOOL_TIMESTAMP_URL%"=="" (
  echo ERROR: SIGNTOOL_TIMESTAMP_URL is not set. Edit set_vendor_certificate.cmd or your machine's environment.
  set RETCODE=-1
  pause
  goto end
)


@rem cross certificate file, same path as this script
set CROSSCERT_FILE="%~dp0%CROSS_CERTIFICATE%"



@rem Activate this to test-sign.
rem goto test_sign

@rem Create an official signature (release signing), adding a timestamp.
@echo.
@echo.
@echo *** Release signing using cert "%VENDOR_CERTIFICATE%" and cross cert %CROSSCERT_FILE%
@echo on
%SIGNTOOLS%\bin\signtool.exe sign /v /n "%VENDOR_CERTIFICATE%" /ac %CROSSCERT_FILE% /t %SIGNTOOL_TIMESTAMP_URL% %*
@set RETCODE=%errorlevel%
@echo off
if not "%RETCODE%"=="0" (
	pause
  goto end
)
goto verify


:test_sign
@rem name of Test Certificate, generated via makecert, see create_test_cert.cmd.
set TEST_CERTIFICATE=My Test Certificate

@rem Creating a self-generated signature (test signing or self signing), adding a timestamp.
@echo.
@echo.
@echo *** Test signing using certificate "%TEST_CERTIFICATE%"
@echo on
%SIGNTOOLS%\bin\signtool.exe sign /v  /n "%TEST_CERTIFICATE%"  /t %SIGNTOOL_TIMESTAMP_URL%  %*
@set RETCODE=%errorlevel%
@echo off
if not "%RETCODE%"=="0" (
	pause
  goto end
)
goto verify


:verify
@rem Check if the signature is a valid kernel-mode driver signature.
@echo.
@echo.
@echo *** Verifying kernel-mode driver signature.
@echo on
%SIGNTOOLS%\bin\signtool.exe verify /kp /tw %*
@set RETCODE=%errorlevel%
@echo off
if not "%RETCODE%"=="0" (
	pause
  goto end
)
goto end



:help
echo.
echo Usage: "%0 <files-to-sign>"
echo.
echo Check set_vendor_certificate.cmd before using this script.
echo.


:end
@exit /b %RETCODE%
