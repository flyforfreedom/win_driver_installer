#pragma once

class CDevManager
{
public:
	CDevManager(void);
	~CDevManager(void);
	bool Mount(TCHAR devLetter, CString& dir_Path);
	bool UnMount(TCHAR devLetter, CString& dir_Path);
};
