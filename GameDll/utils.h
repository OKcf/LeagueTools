#pragma once

#include<memory>
#include<string>
#include "StringConvert.h"


namespace utils
{
	//���Ժ���
	void log(char* _format, ...);

	//���ڴ�����
	template<typename T>
	T read(const DWORD& obj)
	{
		__try{
			T value = *(T*)(obj);
			return value;
		}
		__except (1){
			return T();
		}
	}

	float GetDistance(PEP_POINT_3D pt1, PEP_POINT_3D pt2);
}