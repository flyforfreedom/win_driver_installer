
// DriverInstaller.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDriverInstallerApp:
// �йش����ʵ�֣������ DriverInstaller.cpp
//

class CDriverInstallerApp : public CWinAppEx
{
public:
	CDriverInstallerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDriverInstallerApp theApp;