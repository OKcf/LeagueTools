#pragma once
#include "baseaddr.h"
#include "Monster.h"

namespace GameCall
{
	//��ȡ��Ϸʱ��
	float GetGameTickTime();
	//��ȡ�������
	PEP_POINT_3D GetMousePoint();
	//Ѱ·CALL
	void FindWay(PEP_POINT_3D pt);
	//�չ�CALL
	void Attack(DWORD obj);
	//ʹ�ü���CALL (����Ϊ �������+0F0 ��ǰ���� Ŀ������ �������� ���ܶ���)
	void UseSkill(DWORD dwtargetId, PEP_POINT_3D pCurrPnt, PEP_POINT_3D pDestPnt, DWORD dwSkillIndex, DWORD dwSkillBase);
	//ʹ�ü���CALL 2
	void useSkill2(PEP_POINT_3D pnt, DWORD index, DWORD skObj);
	//����CALL
	bool Defense();
};


void   SkillSkilCrc();
void  FindWaySkipCrc();