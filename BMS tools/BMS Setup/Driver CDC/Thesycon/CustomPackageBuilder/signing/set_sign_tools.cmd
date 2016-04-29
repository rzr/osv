@rem
@rem Set SIGNTOOLS variable that refers to the required tools.
@rem

@rem init
@set SIGNTOOLS=

@rem check if SignTools package is available
@if "%SIGNTOOLS_1_5_0%"=="" (
  @echo ERROR: SIGNTOOLS_1_5_0 environment variable is not set! Please install the SignTools v1.5.0 package.
  @pause
	@exit /b -1
)

@rem We reference all tools through the SIGNTOOLS variable
@set SIGNTOOLS=%SIGNTOOLS_1_5_0%

@exit /b 0
