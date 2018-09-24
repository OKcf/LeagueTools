#include "stdafx.h"
#include "AntiDebug.h"



CAntiDebug::CAntiDebug()
{
}


CAntiDebug::~CAntiDebug()
{
	//�ͷż��������߳�
	if (m_checkDebugerHandle.joinable())
	{
		m_checkDebugerHandle.join();
	}
}

bool CAntiDebug::Initialize()
{
	//����VEH���Ӳ��
	::AddVectoredContinueHandler(1, VectoredHandler);
	//���ü��������̡߳�
	m_checkDebugerHandle = std::thread(std::bind(&CAntiDebug::CheckDebugerPresentThread, this));

	return false;
}

void CAntiDebug::CheckDebugerPresentThread()
{

	while (true)
	{
		//1.����Ƿ񱻵���

		Sleep(5000);
	}
}

bool CAntiDebug::IsDebugerPresent()
{
	bool ret = false;
	__asm
	{
		mov eax, fs: 18h;
		mov eax, [eax + 30h];
		movzx eax, byte ptr[eax + 2];
		//mov ret, eax;
	}
	return ret;
}

LONG CALLBACK VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{

	if (ExceptionInfo->ContextRecord->Dr0 != 0
		|| ExceptionInfo->ContextRecord->Dr1 != 0
		|| ExceptionInfo->ContextRecord->Dr2 != 0
		|| ExceptionInfo->ContextRecord->Dr3 != 0)
	{
		AfxMessageBox("��⵽Ӳ���ϵ�");
		ExceptionInfo->ContextRecord->Dr0 = 0;
		ExceptionInfo->ContextRecord->Dr1 = 0;
		ExceptionInfo->ContextRecord->Dr2 = 0;
		ExceptionInfo->ContextRecord->Dr3 = 0;
	}
	//�����µ�eip �ó����ת��safeִ��
	//ExceptionInfo->ContextRecord->Eip = Vehaddr;
	return EXCEPTION_CONTINUE_EXECUTION;
}
