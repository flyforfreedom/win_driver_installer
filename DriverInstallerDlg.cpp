
// DriverInstallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DriverInstaller.h"
#include "DriverInstallerDlg.h"
#include "UEBODriver.h"
#include <Winsvc.h>
#include <shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDriverInstallerDlg::CDriverInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDriverInstallerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	str_mountFolder = _T("");
}

void CDriverInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDriverInstallerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDINSTALL, &CDriverInstallerDlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDREMOVE, &CDriverInstallerDlg::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_BUTTON1, &CDriverInstallerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDriverInstallerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDriverInstallerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDriverInstallerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDriverInstallerDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CDriverInstallerDlg 消息处理程序

BOOL CDriverInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDriverInstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDriverInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDriverInstallerDlg::OnBnClickedInstall()
{
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!schSCManager) {

		AfxMessageBox(_T("OpenSCManager fail!"));

		return;
    }

	CString str_path = _T("");

	GetDlgItemText(IDC_EDIT1, str_path);

	if(_T("") == str_path){
		AfxMessageBox(_T("Please select a binary file!"));
		CloseServiceHandle(schSCManager);
		return;
	}

	SC_HANDLE s_handle = CreateService(schSCManager, 
		                       _T("UEBO"), 
							   _T("UEBO"),
		                       SERVICE_ALL_ACCESS,    
                               SERVICE_KERNEL_DRIVER, 
                               SERVICE_DEMAND_START,   
                               SERVICE_ERROR_NORMAL,   
                               str_path,    //add later         
                               NULL,                  
                               NULL,                
                               NULL,                  
                               NULL,             
                               NULL  
							   );
	   
	if (s_handle == NULL) {

        if (GetLastError() == ERROR_SERVICE_EXISTS) {

			AfxMessageBox(_T("Service exist!"));

			CloseServiceHandle(schSCManager);

            return;

        } else {

            AfxMessageBox(_T("CreateService fail! %d"), GetLastError());

			CloseServiceHandle(schSCManager);

            return;
        }
	}

	SC_HANDLE s_ohandle = OpenService(schSCManager,
                             _T("UEBO"),
                             SERVICE_ALL_ACCESS
                             );

    if (s_ohandle == NULL) {

        AfxMessageBox(_T("OpenService failed!  Error = %d \n"), GetLastError());	    

		CloseServiceHandle(s_handle);
		
		CloseServiceHandle(schSCManager);

        return;
    }

    if (!StartService(s_ohandle,     // service identifier
                  0,              // number of arguments
                  NULL            // pointer to arguments
                  )) {

		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) {

			AfxMessageBox(_T("Service running!"));	

			CloseServiceHandle(s_handle);
			
			CloseServiceHandle(schSCManager);

			return;

		} else {

			AfxMessageBox(_T("StartService failure!"));

			CString str;
			str.Format(_T("%d"), GetLastError());

			AfxMessageBox(str);

			CloseServiceHandle(s_handle);

			CloseServiceHandle(schSCManager);
			
			return;
		 }
    }
	
	CloseServiceHandle(s_ohandle);
	
	CloseServiceHandle(s_handle);
	
	CloseServiceHandle(schSCManager);

	AfxMessageBox(_T("安装驱动成功! "));

}

void CDriverInstallerDlg::OnBnClickedRemove()
{
	DefineDosDevice(3,
		            _T("U:"),
					DEVICE_NAME);	

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!schSCManager) {

		AfxMessageBox(_T("OpenSCManager fail! %d"), GetLastError());

		return;
    }

	SC_HANDLE s_ohandle = OpenService(schSCManager,
                             _T("UEBO"),
                             SERVICE_ALL_ACCESS
                             );

    if (s_ohandle == NULL) {

        AfxMessageBox(_T("OpenService failed! %d "), GetLastError());

		CloseServiceHandle(schSCManager);

        return;
    }

	SERVICE_STATUS  serviceStatus;

	if (!ControlService(s_ohandle,
                       SERVICE_CONTROL_STOP,
                       &serviceStatus
                       )) {

		CloseServiceHandle(s_ohandle);

		CloseServiceHandle(schSCManager);

		CString str;
		str.Format(_T("%d,%d"), GetLastError(), serviceStatus.dwCurrentState);

		AfxMessageBox(str);
		
		return;
    } 

    if (DeleteService(s_ohandle)) {
    
		AfxMessageBox(_T("删除驱动成功! "));

		CloseServiceHandle(s_ohandle);

		CloseServiceHandle(schSCManager);
		
		return;

    } else {

        AfxMessageBox(_T("DeleteService failed! %d"), GetLastError());

		CloseServiceHandle(s_ohandle);

		CloseServiceHandle(schSCManager);
    }
}

void CDriverInstallerDlg::OnBnClickedButton1()
{
	CFileDialog fileDlg(TRUE, NULL, NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("sys File(*.sys)|*.sys||"));

	if(IDOK == fileDlg.DoModal()){
	
		SetDlgItemText(IDC_EDIT1, fileDlg.GetPathName());
	}
}

void CDriverInstallerDlg::OnBnClickedButton2()
{
	CoInitialize(NULL);

	BROWSEINFO   struInfo; 
	LPCITEMIDLIST   pidl; 
	TCHAR   pszPath[MAX_PATH]; 
	TCHAR   pszTitle[]= "请选择要虚拟成磁盘的文件夹: "; 
	struInfo.hwndOwner=GetSafeHwnd(); 
	struInfo.pszDisplayName=pszPath; 
	struInfo.pidlRoot=NULL; 
	struInfo.ulFlags=BIF_DONTGOBELOWDOMAIN|BIF_RETURNONLYFSDIRS; 
	struInfo.lpfn=NULL; 
	struInfo.lpszTitle=pszTitle; 
	struInfo.iImage=0; 
	pidl=SHBrowseForFolder(&struInfo); 
	if(pidl==NULL) 
	return; 
	if(SHGetPathFromIDList(pidl,pszPath)==FALSE) 
	return; 

	SetDlgItemText(IDC_EDIT2, struInfo.pszDisplayName);

	::CoTaskMemFree((LPVOID)pidl);

	CoUninitialize();
}

void CDriverInstallerDlg::OnBnClickedButton3()
{
	CString str_Folder = _T("");

	GetDlgItemText(IDC_EDIT2, str_Folder);

	if(_T("") == str_Folder){
		
		AfxMessageBox(_T("Please select a folder! "));

		 return;
	}

	if(m_devManager.Mount('U', str_Folder)){
		AfxMessageBox(_T("Mount successfully!"));
		str_mountFolder = str_Folder;
	}
	else{
		AfxMessageBox(_T("Fail to mount!"));
	}
}

void CDriverInstallerDlg::OnBnClickedButton4()
{
	WinExec(_T("explorer U:"), SW_SHOW);
}

void CDriverInstallerDlg::OnBnClickedButton5()
{
	/*if(_T("") != str_mountFolder){*/
		m_devManager.UnMount('U', str_mountFolder);
	/*}*/
}
