
// DriverInstallerDlg.cpp : ʵ���ļ�
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


// CDriverInstallerDlg ��Ϣ�������

BOOL CDriverInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDriverInstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	AfxMessageBox(_T("��װ�����ɹ�! "));

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
    
		AfxMessageBox(_T("ɾ�������ɹ�! "));

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
	TCHAR   pszTitle[]= "��ѡ��Ҫ����ɴ��̵��ļ���: "; 
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
