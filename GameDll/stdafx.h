// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

#define PI 3.1415926563f

//#define _USER_DBG
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "VMProtectSDK.h"


#include <thread>
#include <functional>


//==========================

typedef struct _EP_POINT_3D
{
	float x;
	float z;
	float y;
}EP_POINT_3D, *PEP_POINT_3D;