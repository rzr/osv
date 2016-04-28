/************************************************************************
 *
 *  Module:       cdcacmver.h
 *  Long name:    common version and copyright information
 *  Description:  
 *
 *  Runtime Env.: any
 *
 *  Author(s):   
 *    Udo Eberhardt,  Udo.Eberhardt@thesycon.de
 *                
 *  Companies:
 *    Thesycon GmbH
 *                
 ************************************************************************/

#ifndef __cdcacmver_h__
#define __cdcacmver_h__

//
// Current software version (applies to all modules).
// This indicates the version of the implementation, not of interfaces.
//
#define CURRENT_VERSION_MJ    2      /* major version number */
#define CURRENT_VERSION_MN    0			/* minor version number */
#define CURRENT_VERSION_SUB   0     /* sub version number */


//
// common definitions
//
#define VER_PRODUCTNAME_STR         "USB CDC/ACM Class Driver for Windows"
#define VER_COMPANYNAME_STR         ""
#define VER_LEGALCOPYRIGHT_STR      "\251 2005-2012"


// support macro
#define _TO_STRING_(x)     __TO_STRING__(x)
#define __TO_STRING__(x)   #x


#endif // __cdcacmver_h__

/****************************  EOF **************************************/
