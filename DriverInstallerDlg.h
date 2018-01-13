
// DriverInstallerDlg.h : 头文件
//

#pragma once

#include "DevManager.h"

// CDriverInstallerDlg 对话框
class CDriverInstallerDlg : public CDialog
{
// 构造
public:
	CDriverInstallerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRIVERINSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
