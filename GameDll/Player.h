#pragma once
#include <mutex>
#include <memory>
#include "SkillService.h"
#include "BufferService.h"
#include "Monster.h"
namespace Player
{
	//��ȡ�������
	char* GetName();
	//��ȡӢ������
	char* GetHeroName();
	//��ȡ��ǰѪ��
	float GetCurrHp();
	//��ȡ���Ѫ��
	float GetMaxHp();
	//��ȡ��ǰ����
	float GetCurrMp();
	//��ȡ�������
	float GetMaxMp();
	//��ȡ��ǰ����
	PEP_POINT_3D GetCurrPostion();
	//��ȡ�Ƿ�����
	bool BDead() ;
	//��ȡ�Ƿ�ɼ�
	bool BVisableSee() ;
	//��ȡ�Ƿ��ƶ�
	bool BMoving() ;
	//��ȡ���ﳯ��
	PEP_POINT_3D GetOrientation() ;
	//��ȡ������Ӫ
	DWORD GetCamp() ;
	//��ȡ��������
	DWORD GetType() ;
	//��ȡ��ҵȼ�
	DWORD GetLevel() ;
	//��ȡ������
	float GetAttackPower() ;
	//��ȡ�ƶ��ٶ�
	float GetMoveSpeed() ;
	//��ȡ�����ٶ�
	float GetAttackSpeed() ;
	//��ȡ��������
	float GetAttackDistance() ;
	//��ȡ��������
	Skill* GetSkillByIndex(DWORD index);
	//��ȡ����ֵ +58
	float GetArmour() ;
	//��ȡ���״�͸ +5C
	float GetArmourPenetration();
	//��ȡģ�ʹ�С
	float GetMonsModelSize() ;
	//��ȡ����ǿ��
	float GetSpellPower() ;
	//��ȡ����˹����ʱ��
	float GetXerathPowerTime();
	//��ȡ����˹��ǰ���ܾ���
	float GetXerathSkillCurrDistance();
	//��ȡObject������ʹ��
	DWORD GetObjectForSkill();
	//����Ԥ������
	EP_POINT_3D ClacAnticipationPnt(DWORD mons, float skDelay, float MissileSpeed);
	//�߿�
	void ZouA2Mons(DWORD target, bool tag);
	//���㹥�����
	float sub_10008CC4(float a1);
	//ʹ�ü���1 ������� ���ܶ��� �����ӳ� �����ٶ� �Ƿ��ƽA
	bool UseSkillToMons(DWORD mons, DWORD skIndex,float skDelay, float MissileSpeed,bool bAttack);
	//ʹ�ü���2
	void UseSkillToMons2(DWORD mons, DWORD skIndex, float MissileSpeed, float skDelay);
	//����Ь�Զ�E
	bool KalistaAutoE2Object(DWORD mons);
};

