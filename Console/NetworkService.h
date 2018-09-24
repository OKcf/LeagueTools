#pragma once
#include <mutex>
#include <Windows.h>

class CNetworkService
{
public:
	~CNetworkService();
	static CNetworkService* GetInstance();
	bool Init();

	int Login(char* username, char* password);
	int Register(char* username, char* password);
	int UserAddTime(char* username, char* key, char* recommond , SHORT& days ,  SHORT& pnts);
	bool BValid();
	int UserUnBind();
	char* UserGetValiDate();
	int UserDeductHour(int i);
private:
	CNetworkService();
	CNetworkService(const CNetworkService&)= delete;
	void operator= (const CNetworkService&) = delete;

	//�ͷ�DLL��ϵͳĿ¼
	bool ResDllToWindowsPath();
	//�ڴ����VNC�ļ�
	bool ResToMemoryAndMemoryLoadVnc();
	//��֤�󶨵�Key
	bool InitializeNetServer();
private:
	static std::mutex m_mutex;
	static CNetworkService* m_pInstance;
	HINSTANCE m_hInstVauthDll;
};

