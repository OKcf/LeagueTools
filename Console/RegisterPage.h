#pragma once
#include "afxwin.h"


// CRegisterPage �Ի���

class CRegisterPage : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterPage)

public:
	CRegisterPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterPage();
	enum { IDD = IDD_DIALOG_REGISTER};
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CStatic m_logCtl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_username;
	CString m_password;
};
