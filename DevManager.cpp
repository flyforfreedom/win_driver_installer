#include "StdAfx.h"
#include "DevManager.h"
#include "UEBODriver.h"

#pragma warning(disable:4996)

CDevManager::CDevManager(void)
{
}

CDevManager::~CDevManager(void)
{
}

bool CDevManager::Mount(TCHAR devLetter, CString& dir_Path)
{
	TCHAR volumeName[] = _T("\\\\.\\ :");

	volumeName[4] = devLetter;

    HANDLE handle = CreateFile(volumeName,FILE_ALL_ACCESS, 
		                       FILE_SHARE_READ|FILE_SHARE_WRITE, 
		                       NULL, OPEN_EXISTING, 
							   FILE_ATTRIBUTE_NORMAL
							   |FILE_FLAG_BACKUP_SEMANTICS,
							   NULL);
	if(INVALID_HANDLE_VALUE != handle){
		AfxMessageBox(_T("Already exist!"));
		return false;
	}

	if(!DefineDosDevice(DDD_RAW_TARGET_PATH,
		                &volumeName[4],
						DEVICE_NAME)){
			AfxMessageBox(_T("Create symbol link fail!"));
			return false;
	}

	handle = CreateFile(volumeName,FILE_ALL_ACCESS, 
		                       FILE_SHARE_READ|FILE_SHARE_WRITE, 
		                       NULL, OPEN_EXISTING, 
							   FILE_ATTRIBUTE_NORMAL
							   |FILE_FLAG_BACKUP_SEMANTICS,
							   NULL);
	if(INVALID_HANDLE_VALUE == handle){
		DefineDosDevice(3,
		                &volumeName[4],
						DEVICE_NAME);
		AfxMessageBox(_T("Open volume fail!"));

		DWORD dRet = GetLastError();
		CString str;
		str.Format(_T("%d"), dRet);
        AfxMessageBox(str);

		return false;
	}

	PDIRECTORY_INFORMATION pDir_Info = (PDIRECTORY_INFORMATION)malloc(sizeof(DIRECTORY_INFORMATION)
		+ dir_Path.GetLength() + 8);
	ZeroMemory(pDir_Info, sizeof(DIRECTORY_INFORMATION)
		+ dir_Path.GetLength() + 8);
	
	if(dir_Path.Left(1) == '\\'){
		dir_Path = _T("\\??\\UNC")+dir_Path;
	}
	else{
		dir_Path = _T("\\??\\")+dir_Path;
	}

	pDir_Info->deviceLetter = devLetter;
	pDir_Info->dirNameLength = dir_Path.GetLength();
	pDir_Info->dirSize.QuadPart = 1024*1024; // 1G for example
	pDir_Info->version = 1;

	memcpy(pDir_Info->dir_Name, dir_Path.GetBuffer(), dir_Path.GetLength());
	dir_Path.ReleaseBuffer();

#ifdef UNICODE
	CHAR cPath[256];
	WideCharToMultiByte(CP_ACP, 0, dir_Path.GetBuffer(), 255,
		cPath, 255, NULL, NULL);
	memset(pDir_Info->dir_Name, 0, 256);
	memcpy(pDir_Info->dir_Name,cPath, 255);
    dir_Path.ReleaseBuffer();
#endif

	if(!DeviceIoControl(handle, 
		                IOCTL_FILE_DISK_OPEN_FILE,
						(LPVOID)pDir_Info,
						sizeof(DIRECTORY_INFORMATION)+pDir_Info->dirNameLength - 1,
						(LPVOID)NULL,
						0,
						0,
						NULL)){

        CString str;
		str.Format(_T("%d"), GetLastError());
		AfxMessageBox(str);
		DefineDosDevice(3,
		                &volumeName[4],
						DEVICE_NAME);	
		CloseHandle(handle);

		return false;
	}
	else{
		AfxMessageBox(_T("About to mount volume"));
	}

	CloseHandle(handle);

	return true;
}

bool CDevManager::UnMount(TCHAR devLetter, CString& dir_Path)
{
	TCHAR volumeName[] = _T("\\\\.\\ :");

	volumeName[4] = devLetter;

    HANDLE hDevice = CreateFile(volumeName,FILE_ALL_ACCESS, 
		                       FILE_SHARE_READ|FILE_SHARE_WRITE, 
		                       NULL, OPEN_EXISTING, 
							   FILE_ATTRIBUTE_NORMAL
							   |FILE_FLAG_BACKUP_SEMANTICS,
							   NULL);
	if(INVALID_HANDLE_VALUE == hDevice){
		AfxMessageBox(_T("Volume does not exist!"));
		return false;
	}

    PDIRECTORY_INFORMATION OpenDirInformation = (PDIRECTORY_INFORMATION)malloc(sizeof(DIRECTORY_INFORMATION) + MAX_PATH);

	DWORD BytesReturned = 0;

    if (!DeviceIoControl(
            hDevice,
            IOCTL_FILE_DISK_QUERY_FILE,
            NULL,
            0,
            OpenDirInformation,
            sizeof(DIRECTORY_INFORMATION) + MAX_PATH,
            &BytesReturned,
            NULL
            ))
    {
            free(OpenDirInformation);
            CloseHandle(hDevice);

			AfxMessageBox(_T("IOCTL_FILE_DISK_QUERY_FILE fail!"));

            return false;
    }

    free(OpenDirInformation);

    if (!DeviceIoControl(
            hDevice,
            FSCTL_LOCK_VOLUME,
            NULL,
            0,
            NULL,
            0,
            &BytesReturned,
            NULL
            ))
    {
		AfxMessageBox(_T("FSCTL_LOCK_VOLUME fail!"));
         
		return false;
    }

    if (!DeviceIoControl(
            hDevice,
            IOCTL_FILE_DISK_CLOSE_FILE,
            NULL,
            0,
            NULL,
            0,
            &BytesReturned,
            NULL
            ))
    {
		AfxMessageBox(_T("IOCTL_FILE_DISK_CLOSE_FILE fail!"));

        return false;
    }

    if (!DeviceIoControl(
            hDevice,
            FSCTL_DISMOUNT_VOLUME,
            NULL,
            0,
            NULL,
            0,
            &BytesReturned,
            NULL
            ))
    {
		AfxMessageBox(_T("FSCTL_DISMOUNT_VOLUME fail!"));

        return false;
    }

    if (!DeviceIoControl(
            hDevice,
            FSCTL_UNLOCK_VOLUME,
            NULL,
            0,
            NULL,
            0,
            &BytesReturned,
            NULL
            ))
    {
		AfxMessageBox(_T("FSCTL_UNLOCK_VOLUME fail!"));

		return false;
    }

    CloseHandle(hDevice);

    if (!DefineDosDevice(
            3,
            &volumeName[4],
            NULL
            ))
    {
		AfxMessageBox(_T("DefineDosDevice fail!"));

        return false;
    }

	AfxMessageBox(_T("UnMount finish!"));

	return true;
}
