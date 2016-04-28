/*
	common version resources
	used by all software modules

  Author(s):    Udo Eberhardt
  Company:      Thesycon GmbH
*/

#ifdef APSTUDIO_INVOKED
#error This file is not editable by Microsoft Visual C++, use a text editor.
#endif

#include <winver.h>

#ifndef VER_FILETYPE
#define	VER_FILETYPE      VFT_UNKNOWN
#endif
/* possible values:		
        VFT_UNKNOWN
				VFT_APP
				VFT_DLL
				VFT_DRV
				VFT_FONT
				VFT_VXD
				VFT_STATIC_LIB
*/


#define	VER_FILESUBTYPE   VFT2_UNKNOWN
/* possible values		
        VFT2_UNKNOWN
				VFT2_DRV_PRINTER
				VFT2_DRV_KEYBOARD
				VFT2_DRV_LANGUAGE
				VFT2_DRV_DISPLAY
				VFT2_DRV_MOUSE
				VFT2_DRV_NETWORK
				VFT2_DRV_SYSTEM
				VFT2_DRV_INSTALLABLE
				VFT2_DRV_SOUND
				VFT2_DRV_COMM
*/


/* version number string format definition */

#define _VERSION_NUMBER_STRING          \
    _TO_STRING_(CURRENT_VERSION_MJ)     \
    "."                                 \
    _TO_STRING_(CURRENT_VERSION_MN)			\
    "."                                 \
    _TO_STRING_(CURRENT_VERSION_SUB)		\
    "."                                 \
    "0"

#if DBG || defined(_DEBUG)
#define VERSION_NUMBER_STRING   _VERSION_NUMBER_STRING " debug"
#else
#define VERSION_NUMBER_STRING   _VERSION_NUMBER_STRING
#endif


#ifndef VER_BUILDTYPE_EXT_STRING
#define VER_BUILDTYPE_EXT_STRING	""
#endif

// value of field BuildType
#if DBG || defined(_DEBUG)
#define VER_BUILDTYPE_STRING    "debug" VER_BUILDTYPE_EXT_STRING
#else
#define VER_BUILDTYPE_STRING    "release" VER_BUILDTYPE_EXT_STRING
#endif



/*-----------------------------------------------*/
/* version information                           */
/*-----------------------------------------------*/

#define VER_FILEVERSION         CURRENT_VERSION_MJ,CURRENT_VERSION_MN,CURRENT_VERSION_SUB,0
#define VER_FILEVERSION_STR     VERSION_NUMBER_STRING

#define VER_PRODUCTVERSION      VER_FILEVERSION
#define VER_PRODUCTVERSION_STR  VER_FILEVERSION_STR


/*--------------------------------------------------------------*/
/* the following section defines values used in the version     */
/* data structure for all files, and which do not change.       */
/*--------------------------------------------------------------*/

/* default is nodebug */
#if DBG || defined(_DEBUG)
#define VER_DEBUG                   VS_FF_DEBUG
#else
#define VER_DEBUG                   0
#endif

/* default is no prerelease */
#if BETA
#define VER_PRERELEASE              VS_FF_PRERELEASE
#else
#define VER_PRERELEASE              0
#endif

#define VER_FILEFLAGSMASK           VS_FFI_FILEFLAGSMASK
#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILEFLAGS               (VER_PRERELEASE|VER_DEBUG)



/*---------------------------------------------------------------*/
/*                                                               */
/* The following section actually creates the version structure. */
/* They are ignored if we are not being invoked by RC.           */
/*                                                               */
/*---------------------------------------------------------------*/

#ifdef RC_INVOKED

VS_VERSION_INFO VERSIONINFO
FILEVERSION    VER_FILEVERSION
PRODUCTVERSION VER_PRODUCTVERSION
FILEFLAGSMASK  VER_FILEFLAGSMASK
FILEFLAGS      VER_FILEFLAGS
FILEOS         VER_FILEOS
FILETYPE       VER_FILETYPE
FILESUBTYPE    VER_FILESUBTYPE
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040904B0"   /* LANG_ENGLISH/SUBLANG_ENGLISH_US, Unicode CP */
        BEGIN
            VALUE "CompanyName",     VER_COMPANYNAME_STR
            VALUE "FileDescription", VER_FILEDESCRIPTION_STR
            VALUE "FileVersion",     VER_FILEVERSION_STR
            VALUE "InternalName",    VER_INTERNALNAME_STR
            VALUE "LegalCopyright",  VER_LEGALCOPYRIGHT_STR
            VALUE "OriginalFilename",VER_ORIGINALFILENAME_STR
            VALUE "ProductName",     VER_PRODUCTNAME_STR
            VALUE "ProductVersion",  VER_PRODUCTVERSION_STR
            VALUE "BuildType",		   VER_BUILDTYPE_STRING
#ifdef UNDER_CE
            VALUE "Platform",		"Windows CE"
#else
#ifdef _WIN64
            VALUE "Platform",		"Windows x64"
#else
            VALUE "Platform",		"Windows x86"
#endif
#endif
#ifdef VER_API_VERSION_STR
            VALUE "API Version", VER_API_VERSION_STR     
#endif
#ifdef VER_DEMO_TIMEOUT_MINUTES_STR
            VALUE "Demo Timeout",	VER_DEMO_TIMEOUT_MINUTES_STR
#endif
        END

    END

    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x0409, 0x04B0
    END
END

#endif



/*************************** EOF **************************************/
