
// DriverInstallerDlg.h : ͷ�ļ�
//

#pragma once

#include "DevManager.h"

// CDriverInstallerDlg �Ի���
class CDriverInstallerDlg : public CDialog
{
// ����
public:
	CDriverInstallerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DRIVERINSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInstall();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

private:

	CDevManager m_devManager;
	CString str_mountFolder;

public:
	afx_msg void OnBnClickedButton5();
};
