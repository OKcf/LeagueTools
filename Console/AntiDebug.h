#pragma once
/*
���ط�����
*/
#include <thread>
#include <functional>

class CAntiDebug
{
public:
	CAntiDebug();
	~CAntiDebug();

	
	bool Initialize();

	//���������߳�
	void CheckDebugerPresentThread();
private:
	//��������
	bool IsDebugerPresent();
	//���Ӳ���ϵ�


private:
	std::thread m_checkDebugerHandle;
};

LONG CALLBACK VectoredHandler(
	_In_ PEXCEPTION_POINTERS ExceptionInfo
);

