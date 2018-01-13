///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2010 - UIT
///
/// Original filename: UEBODriver.h
/// Project          : UEBODriver
/// Date of creation : 2010-10-14
/// Author(s)        : Wei Shun
///
/// Purpose          : Virtual disk driver
///
/// Revisions:       0000 [2010-10-14] Initial revision.
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __UEBODRIVER_H_VERSION__
#define __UEBODRIVER_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include<winioctl.h>

#define DEVICE_NAME			"\\Device\\UEBO"
#define SYMLINK_NAME		"\\DosDevices\\UEBO"

#ifndef FILE_DEVICE_UEBODRIVER
#define FILE_DEVICE_UEBODRIVER 0x8000
#endif

#define IOCTL_FILE_DISK_OPEN_FILE   CTL_CODE(FILE_DEVICE_UEBODRIVER, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_CLOSE_FILE  CTL_CODE(FILE_DEVICE_UEBODRIVER, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_QUERY_FILE  CTL_CODE(FILE_DEVICE_UEBODRIVER, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS)

typedef struct _DIRECTORY_INFORMATION {
	USHORT			version;
    LARGE_INTEGER   dirSize;
    USHORT          dirNameLength;
    UCHAR			deviceLetter;
	UCHAR           dir_Name[1];
} DIRECTORY_INFORMATION, *PDIRECTORY_INFORMATION;

#endif // __UEBODRIVER_H_VERSION__
