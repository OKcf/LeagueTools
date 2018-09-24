// LoginPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Console.h"
#include "LoginPage.h"
#include "afxdialogex.h"
#include "RegisterPage.h"
#include "AddtimePage.h"
#include "NetworkService.h"
#include "MainPage.h"

#define WM_LOGIN WM_USER+100
#define WM_SETLOG WM_USER+101
// CLoginPage �Ի���
#include <sstream>
IMPLEMENT_DYNAMIC(CLoginPage, CDialogEx)

CLoginPage::CLoginPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
	, m_bRemberPassword(TRUE)
{

}

CLoginPage::~CLoginPage()
{
}

void CLoginPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_password);
	DDX_Check(pDX, IDC_CHECK1, m_bRemberPassword);
	DDX_Control(pDX, IDC_STATIC_LOG, m_logCtl);
}


BEGIN_MESSAGE_MAP(CLoginPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginPage::OnBnClickedButtonLogin)
	ON_MESSAGE(WM_LOGIN, &CLoginPage::OnLogin)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CLoginPage::OnBnClickedButton3)
	ON_MESSAGE(WM_SETLOG, &CLoginPage::OnSetlog)
END_MESSAGE_MAP()


// CLoginPage ��Ϣ�������


void CLoginPage::OnBnClickedButtonLogin()
{
	// TODO: �����¼�߼�
	VMProtectBegin(__FUNCTION__);
	UpdateData(TRUE);
	::SendMessage(m_hWnd, WM_LOGIN, (WPARAM)(m_username.GetBuffer(m_username.GetLength())), (LPARAM)m_password.GetBuffer(m_password.GetLength()));
	m_username.ReleaseBuffer();
	m_password.ReleaseBuffer();
	VMProtectEnd();
}


afx_msg LRESULT CLoginPage::OnLogin(WPARAM wParam, LPARAM lParam)
{
	char* username = (char*)(wParam);
	char* password = (char*)(lParam);
	auto ret = CNetworkService::GetInstance()->Login(username, password);
	switch (ret)
	{
		case 0: //��¼�ɹ����л���������
		{
			//�����֤��Ч���л���������
			if (CNetworkService::GetInstance()->BValid())
			{
				std::stringstream ss;
				ss << m_bRemberPassword;
				WritePrivateProfileString("Account", "username", m_username, ".\\acc.ini");
				WritePrivateProfileString("Account", "password", m_password, ".\\acc.ini");
				WritePrivateProfileString("Account", "bRemberPassword", ss.str().c_str(), ".\\acc.ini");

				VMProtectBegin(__FUNCTION__);
				CDialog::OnOK();
				CMainPage dlg;
				theApp.m_pMainWnd = &dlg;
				dlg.DoModal();
				VMProtectEnd();
			}

			break;
		}
		case -3: {
			CString temp = "�����󶨻�������";
			::SendMessage(m_hWnd, WM_SETLOG, NULL, (LPARAM)(&temp));
			break;
		}
		case -4: {
			CString temp = "�û��Ѿ�����";
			::SendMessage(m_hWnd, WM_SETLOG, NULL, (LPARAM)(&temp));
			break;
		}
		case -1: 
		case -2: 
		case -5: 
		case -6: 
		case -7:
		default:
			CString temp = "δ֪��¼����";
			::SendMessage(m_hWnd, WM_SETLOG, NULL, (LPARAM)(&temp));
			break;
	}
	return 0;
}


void CLoginPage::OnBnClickedButton2()
{
	// TODO: �л���ע��ҳ��

	CDialog::OnOK();

	CRegisterPage dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
}


void CLoginPage::OnBnClickedButton3()
{
	// TODO: �л�����ֵҳ��
	CDialog::OnOK();

	CAddtimePage dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
}



afx_msg LRESULT CLoginPage::OnSetlog(WPARAM wParam, LPARAM lParam)
{
	CString pDes = *(CString*)(lParam);
	m_logCtl.SetWindowTextA(pDes);
	return 0;
}


BOOL CLoginPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//��ȡ�����ļ�
	if (1 == GetPrivateProfileInt("Account", "bRemberPassword", 0, ".\\acc.ini"))
	{
		//��ȡ�����ļ�
		char username[32] = { 0 };
		char password[32] = { 0 };
		GetPrivateProfileString("Account", "username", "", username, 32, ".\\acc.ini");
		GetPrivateProfileString("Account", "password", "", password, 32, ".\\acc.ini");
		if (0 == CNetworkService::GetInstance()->Login(username, password))
		{
			if (CNetworkService::GetInstance()->BValid())
			{
				VMProtectBegin(__FUNCTION__);
				CDialog::OnOK();
				CMainPage dlg;
				theApp.m_pMainWnd = &dlg;
				dlg.DoModal();
				VMProtectEnd();
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
