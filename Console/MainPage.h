#pragma once
#include "afxwin.h"


// CMainPage �Ի���

class CMainPage : public CDialogEx
{
	DECLARE_DYNAMIC(CMainPage)

public:
	CMainPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainPage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
//	CStatic m_EndDateTextCtl;
	virtual BOOL OnInitDialog();
//	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_bmpStaticCtl;
	afx_msg void OnStnDblclickStaticbmp();
	static NOTIFYICONDATA m_nid; //��ʼ���������
	static HMENU m_hMenu; //���̲˵�
protected:
	afx_msg LRESULT OnInject(WPARAM wParam, LPARAM lParam);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg LRESULT OnTray(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
};

DWORD WINAPI InjectThreadProc(
	_In_ LPVOID lpParameter
);