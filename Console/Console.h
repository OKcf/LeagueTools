
// Console.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "AntiDebug.h"

// CConsoleApp: 
// �йش����ʵ�֣������ Console.cpp
//

class CConsoleApp : public CWinApp
{
public:
	CConsoleApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
	CAntiDebug m_antiDebuger;
};

extern CConsoleApp theApp;