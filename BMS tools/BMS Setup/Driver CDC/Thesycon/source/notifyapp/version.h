/************************************************************************
 *
 *  Module:       version.h
 *  Description:
 *     contains the version and build number constants
 *
 *  Author(s):   
 *    Udo Eberhardt
 *                
 *  Companies:
 *    Thesycon GmbH, Germany      http://www.thesycon.de
 *                
 ************************************************************************/

#ifndef __version_h__
#define __version_h__

// common version definition
#include "cdcacmver.h"


// filename
#define VER_INTERNALNAME_STR        _TO_STRING_(TARGET_FILE_NAME)
#define VER_ORIGINALFILENAME_STR    _TO_STRING_(TARGET_FILE_NAME)

// description
#define VER_FILEDESCRIPTION_STR     "Thesycon TEMPLATE Console App"


// file type
#define VER_FILETYPE      VFT_APP
/* possible values:   
        VFT_UNKNOWN
        VFT_APP
        VFT_DLL
        VFT_DRV
        VFT_FONT
        VFT_VXD
        VFT_STATIC_LIB
*/


// support macro
#define _TO_STRING_(x)     __TO_STRING__(x)
#define __TO_STRING__(x)   #x


#endif  // __version_h__

/*************************** EOF **************************************/
