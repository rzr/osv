@echo off
setlocal

@rem set variables
call ".\signing\set_sign_tools.cmd"
if not "%errorlevel%"=="0" goto end

@rem run INF checker
if EXIST driver_package (
	cmd /c %SIGNTOOLS%\ChkInf\chkinf.bat "driver_package\dbg\*.inf" "driver_package\rel\*.inf" /A /B
)

if EXIST driver_package_notsigned (
	cmd /c %SIGNTOOLS%\ChkInf\chkinf.bat "driver_package_notsigned\dbg\*.inf" "driver_package_notsigned\rel\*.inf" /A /B
)


:end
endlocal
