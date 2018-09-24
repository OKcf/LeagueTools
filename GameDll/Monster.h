#pragma once


namespace Monster
{
	//��ȡ�������
	char* GetName(DWORD obj);
	//��ȡӢ������
	char* GetHeroName(DWORD obj);
	//��ȡ��ǰѪ��
	float GetCurrentHealth(DWORD obj);
	//��ȡ���Ѫ��
	float GetMaxHealth(DWORD obj);
	//��ȡ��ǰ����
	float GetCurrentMagic(DWORD obj);
	//��ȡ�������
	float GetMaxMagic(DWORD obj);
	//��ȡ��ǰ����
	PEP_POINT_3D GetCurrentPos(DWORD obj);
	//��ȡģ������
	PEP_POINT_3D GetModelPos(DWORD obj);
	//��ȡ�Ƿ�������־λ
	bool BDead(DWORD obj);
	//��ȡ�Ƿ��ƶ���־λ
	bool BMoving(DWORD obj);
	//��ȡ�Ƿ�ɼ���־λ
	bool BVisable(DWORD obj);
	//��ȡ��������
	DWORD GetType(DWORD obj);
	//��ȡ������Ӫ
	DWORD GetCamp(DWORD obj);
	//��ȡ���ﳯ��
	PEP_POINT_3D GetOrientation(DWORD obj);
	//��ȡ������
	float GetDefensePower(DWORD obj);
	//��ȡ�ƶ��ٶ�
	float GetMoveSpeed(DWORD obj);
	//��ȡ���ܲ���
	DWORD GetObjectForSkill(DWORD obj);
	//��ȡģ�ʹ�С
	float GetModelWidth(DWORD obj);
}