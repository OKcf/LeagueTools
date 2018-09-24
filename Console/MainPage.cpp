// MainPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Console.h"
#include "MainPage.h"
#include "afxdialogex.h"
#include "NetworkService.h"
#include "LoadLibraryR.h"
// CMainPage �Ի���
#define WM_INJECT WM_USER+1105
IMPLEMENT_DYNAMIC(CMainPage, CDialogEx)
#define ID_SHOW     40001  
#define ID_EXIT     40002  
#define WM_TRAY (WM_USER + 100)  
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated"))  
NOTIFYICONDATA CMainPage::m_nid = {0};
HMENU CMainPage::m_hMenu; //���̲˵�
CMainPage::CMainPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{

}

CMainPage::~CMainPage()
{
}

void CMainPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_bmp, m_bmpStaticCtl);
}


BEGIN_MESSAGE_MAP(CMainPage, CDialogEx)
	ON_WM_CLOSE()
//	ON_WM_PAINT()
ON_WM_TIMER()
ON_STN_DBLCLK(IDC_STATIC_bmp, &CMainPage::OnStnDblclickStaticbmp)
ON_MESSAGE(WM_INJECT, &CMainPage::OnInject)
ON_MESSAGE(WM_TRAY, &CMainPage::OnTray)
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainPage ��Ϣ�������


void CMainPage::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (IDOK == ::MessageBox(m_hWnd,"�Ƿ����󶨣�����󶨻�۳�2��Сʱ","��ʾ", MB_YESNO))
	{
		if (0 == CNetworkService::GetInstance()->UserUnBind())
		{
			CNetworkService::GetInstance()->UserDeductHour(2);
		}
	}

	::Shell_NotifyIcon(NIM_DELETE, &m_nid);

	CDialogEx::OnClose();
}


BOOL CMainPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//CString _format;
	//_format.Format("����ʱ�䣺%s", CNetworkService::GetInstance()->UserGetValiDate());
	//SetWindowText(_format);

	auto hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle() ,
		MAKEINTRESOURCE(IDB_BITMAP_BOCK) ,
		IMAGE_BITMAP, 
		0, 0,
		LR_LOADMAP3DCOLORS);
	m_bmpStaticCtl.ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	m_bmpStaticCtl.SetBitmap(hBitmap);

	
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	m_nid.uCallbackMessage = WM_TRAY;
	m_nid.hIcon = LoadIcon(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	lstrcpy(m_nid.szTip, "zx1223a");

	m_hMenu = CreatePopupMenu();//�������̲˵�  
								//Ϊ���̲˵��������ѡ��  
	AppendMenu(m_hMenu, MF_STRING, ID_SHOW, TEXT("��ʾ"));
	AppendMenu(m_hMenu, MF_STRING, ID_EXIT, TEXT("�˳�"));

	::Shell_NotifyIcon(NIM_ADD, &m_nid);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//void CMainPage::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: �ڴ˴������Ϣ����������
//					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
//}


void CMainPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1000:
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMainPage::OnStnDblclickStaticbmp()
{
	// TODO: �û�˫����ͼƬ����ִ��ע�������
	if (CNetworkService::GetInstance()->BValid())
	{
		::SendMessage(m_hWnd, WM_INJECT, NULL, NULL);
		::ShowWindow(m_hWnd, SW_HIDE);
	}
}

DWORD WINAPI InjectThreadProc(LPVOID lpParameter)
{
	DWORD pid = 0;

	if (!CNetworkService::GetInstance()->BValid())
	{
		return 0;
	}

	VMProtectBegin(__FUNCTION__);
	auto hwnd = ::FindWindow("RiotWindowClass", NULL);
	if (!hwnd)
	{
		return 0;
	}

	GetWindowThreadProcessId(hwnd, &pid);
	if (!pid)
	{
		return 0;
	}

	HRSRC hIdrDllALLp = ::FindResource(NULL, MAKEINTRESOURCE(IDR_DLL_MEM), "DLL");
	if (!hIdrDllALLp)
	{
		return 0;
	}

	auto IdrSize = SizeofResource(NULL, hIdrDllALLp);
	if (!IdrSize) {
		return 0;
	}

	HGLOBAL hGlobal = LoadResource(NULL, hIdrDllALLp);
	if (!hGlobal) {
		return 0;
	}

	LPVOID pBuffer = LockResource(hGlobal);
	if (!pBuffer) {
		return 0;
	}

	LPVOID lpBuffer = HeapAlloc(GetProcessHeap(), 0, IdrSize);
	if (!lpBuffer) {
		return 0;
	}

	memcpy(lpBuffer, pBuffer, IdrSize);

	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES priv = { 0 };
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
	if (!hProcess)
	{
		return 0;
	}
	auto hModule = LoadRemoteLibraryR(hProcess, lpBuffer, IdrSize, NULL);
	if (!hModule)
	{
		return 0;
	}

	WaitForSingleObject(hModule, -1);
	if (lpBuffer)
	{
		HeapFree(GetProcessHeap(), 0, lpBuffer);
	}
	if (hProcess)
	{
		CloseHandle(hProcess);
	}


	VMProtectEnd();
	return 1;
}


afx_msg LRESULT CMainPage::OnInject(WPARAM wParam, LPARAM lParam)
{
	static DWORD timeSec = 0;
	if (GetTickCount() - timeSec > 15000)
	{
		auto handle = ::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(InjectThreadProc), NULL, NULL, NULL);
		CloseHandle(handle);
		timeSec = GetTickCount();
	}
	return  true;
}


//BOOL CMainPage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//
//	return CDialogEx::OnNotify(wParam, lParam, pResult);
//}


afx_msg LRESULT CMainPage::OnTray(WPARAM wParam, LPARAM lParam)
{
	if (m_nid.uID != wParam) {
		return 0;
	}

	switch (lParam)
	{
	case WM_LBUTTONDBLCLK:
		::ShowWindow(m_hWnd, SW_SHOW);
		break;
	case WM_RBUTTONDOWN: {
		//��ȡ�������  
		POINT pt; GetCursorPos(&pt);

		//����ڲ˵��ⵥ������˵�����ʧ������  
		::SetForegroundWindow(m_hWnd);

		//ʹ�˵�ĳ����  
		//EnableMenuItem(hMenu, ID_SHOW, MF_GRAYED);      

		//��ʾ����ȡѡ�еĲ˵�  
		int cmd = TrackPopupMenu(m_hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, m_hWnd, NULL);
		if (cmd == ID_SHOW)
		{
			::ShowWindow(m_hWnd, SW_SHOW);
			return true;
		}

		if (cmd == ID_EXIT)
		{
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
			return true;
		}
	}

	default:
		break;
	}
	return 0;
}


void CMainPage::OnDestroy()
{

	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������	


}
