#include "stdafx.h"
#include "Player.h"
#include "baseaddr.h"
#include "utils.h"
#include "GameCall.h"
#include "HookToMainThread.h"
#include "GameMenu.h"

#include <algorithm>

//ȫ�ֱ���
std::shared_ptr<CSkillService> m_skService = nullptr;
std::shared_ptr<CBufferService> m_buffService = nullptr;
float m_ZouKanTimeSec[2] = {0};


void Player::ZouA2Mons(DWORD target, bool tag)
{
	static bool bDefense = false;
	//timeSec[0]������ҡ  timeSec[1]����ǰҡ
	if (GameCall::GetGameTickTime() > m_ZouKanTimeSec[1])
	{
		if (target && tag)
		{
			//�߿������㹥��ǰ��ҡ
			DWORD monObj = target;
			//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_ATTACKCALL , (LPARAM)&monObj);
			GameCall::Attack(monObj);

			auto attackSpeed = Player::GetAttackSpeed();
			m_ZouKanTimeSec[0] = GameCall::GetGameTickTime() + (attackSpeed - attackSpeed * sub_10008CC4(attackSpeed)) / 1.5f;
			m_ZouKanTimeSec[1] = GameCall::GetGameTickTime() + attackSpeed;
			bDefense = true;
		}
		else
		{
			CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_FINDWAY, NULL);
		}
	}
	else if (GameCall::GetGameTickTime() > m_ZouKanTimeSec[0])
	{
		//����ﵽ������ҡʱ��
		if (bDefense)
		{
			CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_DEFENSE, NULL);
			bDefense = false;
		}
		CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_FINDWAY, NULL);
	}

}

float Player::sub_10008CC4(float a1)
{

	if (a1 >= 1.7f) {
		return 0.65f;
	}
	else if (a1 < 1.7f && a1 > 0.3f) {
		return (a1 - 0.25f) * 0.2f + 0.4f;
	}
	else {
		return 0.2f;
	}
}

bool Player::UseSkillToMons(DWORD mons, DWORD skIndex, float skDelay, float MissileSpeed, bool bAttack)
{

	//���ʱ��С�ڹ�����ҡʱ�䣬��ʹ�ü���
	if (GameCall::GetGameTickTime() < m_ZouKanTimeSec[0]){
		return false;
	}

	//�жϹ����Ƿ����
	if (!mons){
		return false;
	}

	//����������� || ���ﲻ�ɼ�
	if (Monster::BDead(mons) || !Monster::BVisable(mons)){
		return false;
	}

	auto sk = GetSkillByIndex(skIndex);
	if (!sk){
		return false;
	}

	//�жϼ����Ƿ����
	if (sk->GetLevel() < 1 || !sk->BCoolDown() || Player::GetCurrMp() < sk->GetExpendMp()){
		return false;
	}


	//���ݼ�������ȷ�����ܲ���
	switch (sk->GetSkillType())
	{
	case SK_SKILL_LINE :  //ָ���Լ��ܣ�����Q
	{
		auto pnt = Player::ClacAnticipationPnt(mons, skDelay, MissileSpeed);
		auto dis = sk->GetDistance();
		if (utils::GetDistance(Player::GetCurrPostion(), &pnt) > dis * dis) {
			return false;
		}

		SKILL_TO_MONS sk2mons = { 0 };
		memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		memcpy(&sk2mons.pAskPnt, &pnt, sizeof(EP_POINT_3D));

		//Ȼ��Ѽ��������Բ��ֱ�ߵĽ�����Ϊ�ڶ�������
		float angles = atan2(Monster::GetCurrentPos(mons)->y - Player::GetCurrPostion()->y, Monster::GetCurrentPos(mons)->x - Player::GetCurrPostion()->x);
		sk2mons.pDestPnt.x = cos(angles) * sk->GetDistance() + Player::GetCurrPostion()->x;
		sk2mons.pDestPnt.y = sin(angles) * sk->GetDistance() + Player::GetCurrPostion()->y;
		sk2mons.pDestPnt.z = Monster::GetCurrentPos(mons)->z;

		sk2mons.targetId = 0;
		sk2mons.skillIndex = skIndex;
		sk2mons.skillBase = sk->GetNodeBase();
		//�����ɣ���ʼ����
		GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
		//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		return true;
	}
	case SK_SKILL_LOCK :   //�������ܣ����簲��Q
	{
		//���
		SKILL_TO_MONS sk2mons = { 0 };
		memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		memcpy(&sk2mons.pDestPnt, Monster::GetCurrentPos(mons), sizeof(EP_POINT_3D));
		sk2mons.targetId = Monster::GetObjectForSkill(mons);
		sk2mons.skillIndex = skIndex;
		sk2mons.skillBase = sk->GetNodeBase();

		//����
		GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
		//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		return true;
	}
	case SK_SKILL_SECTOR :  //���μ��ܣ����簲��W
	{
		auto pnt = Player::ClacAnticipationPnt(mons, skDelay, MissileSpeed);
		auto dis = sk->GetDistance();
		if (utils::GetDistance(Player::GetCurrPostion(), &pnt) > dis * dis) {
			return false;
		}
		//���
		SKILL_TO_MONS sk2mons = { 0 };
		memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		memcpy(&sk2mons.pDestPnt, &pnt, sizeof(EP_POINT_3D));
		memcpy(&sk2mons.pAskPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		sk2mons.targetId = 0;
		sk2mons.skillIndex = skIndex;
		sk2mons.skillBase = sk->GetNodeBase();
		//����
		GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
		//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		return true;
	}
	case SK_SKILL_ROUND : //Բ�μ��ܣ�����R ,����R
	case SK_SKILL_ROUND2: //����˹R , ����Q
	{
		auto pnt = Player::ClacAnticipationPnt(mons, skDelay, MissileSpeed);
		auto dis = sk->GetDistance();
		if (utils::GetDistance(Player::GetCurrPostion(), &pnt) > dis * dis) {
			return false;
		}
		//���
		SKILL_TO_MONS sk2mons = { 0 };
		memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		memcpy(&sk2mons.pDestPnt, &pnt, sizeof(EP_POINT_3D));
		sk2mons.targetId = 0;
		sk2mons.skillIndex = skIndex;
		sk2mons.skillBase = sk->GetNodeBase();
		//����
		GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
		//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		return true;
	}
	case SK_SKILL_TOSELF: //���Լ��ͷŵļ���
	{

		SKILL_TO_MONS sk2mons = { 0 };
		sk2mons.targetId = GetObjectForSkill();
		memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		memcpy(&sk2mons.pDestPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
		sk2mons.skillIndex = sk->GetIndex();
		sk2mons.skillBase = sk->GetNodeBase();
		GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
		//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		return true;
	}
	case SK_SKILL_VIKTOR: //ά���е�E
	{
		auto pnt = Player::ClacAnticipationPnt(mons, skDelay, MissileSpeed);
		auto dis = sk->GetDistance();
		if (utils::GetDistance(Player::GetCurrPostion(), &pnt) > dis * dis) {
			return false;
		}
		//������������֮��ľ��� > ���ܷ�Χ
		auto dis2Mons = sk->GetDistance() / 2.0f;
		if (utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(mons)) < dis2Mons * dis2Mons){
			SKILL_TO_MONS sk2mons = { 0 };
			memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
			memcpy(&sk2mons.pAskPnt, &pnt, sizeof(EP_POINT_3D));
			memcpy(&sk2mons.pDestPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
			sk2mons.targetId = 0;
			sk2mons.skillIndex = skIndex;
			sk2mons.skillBase = sk->GetNodeBase();
			//����
			GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
			//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		}
		else {
			SKILL_TO_MONS sk2mons = { 0 };
			memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
			memcpy(&sk2mons.pAskPnt, &pnt, sizeof(EP_POINT_3D));
			float angles = atan2(Monster::GetCurrentPos(mons)->y - Player::GetCurrPostion()->y, Monster::GetCurrentPos(mons)->x - Player::GetCurrPostion()->x);
			sk2mons.pDestPnt.x = cos(angles) * dis2Mons + Player::GetCurrPostion()->x;
			sk2mons.pDestPnt.y = sin(angles) * dis2Mons + Player::GetCurrPostion()->y;
			sk2mons.pDestPnt.z = Monster::GetCurrentPos(mons)->z;

			sk2mons.targetId = 0;
			sk2mons.skillIndex = skIndex;
			sk2mons.skillBase = sk->GetNodeBase();
			//����		
			GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
			//CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL, (LPARAM)(&sk2mons));
		}
		return true;
	}
	default:
		break;
	}
	return false;
}

void Player::UseSkillToMons2(DWORD mons, DWORD skIndex, float MissileSpeed, float skDelay)
{
	//�жϹ����Ƿ����
	if (!mons)
	{
		return;
	}

	auto sk = GetSkillByIndex(skIndex);
	if (!sk)
	{
		return;
	}

	//�жϼ����Ƿ����
	if (sk->GetLevel() < 1 || !sk->BCoolDown() /*|| Player::GetCurrMp() < sk->GetExpendMp()*/)
	{
		return;
	}

	//����������� || ���ﲻ�ɼ�
	if (Monster::BDead(mons) || !Monster::BVisable(mons))
	{
		return;
	}

	//������������֮��ľ��� > ���ܷ�Χ
	auto dis = sk->GetDistance() + Monster::GetModelWidth(mons);
	if (utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(mons)) > dis * dis)
	{
		return;
	}

	SKILL_TO_MONS sk2mons = { 0 };
	memcpy(&sk2mons.pCurrPnt, Player::GetCurrPostion(), sizeof(EP_POINT_3D));
	EP_POINT_3D pnt = Player::ClacAnticipationPnt(mons, skDelay, MissileSpeed);
	if (pnt.x < 1 && pnt.y < 1) {
		return;
	}
	memcpy(&sk2mons.pAskPnt, &pnt, sizeof(EP_POINT_3D));
	sk2mons.targetId = 0;
	sk2mons.skillIndex = skIndex;
	sk2mons.skillBase = sk->GetNodeBase();
	//�����ɣ���ʼ����
	CHookToMainThread::GetInstance()->SendMessageToMainThread(MSG_USESKILL2, (LPARAM)(&sk2mons));
}

bool  Player::KalistaAutoE2Object(DWORD mons)
{
	if (!mons)
	{
		return false;
	}

	if (Monster::BDead(mons) || !Monster::BVisable(mons) || Monster::GetCurrentHealth(mons) < 4.0f)
	{
		return false;
	}

	CBufferService tempBufferService(mons);
	tempBufferService.travse();

	for (auto buff : tempBufferService.GetBufferList())
	{
		if (nullptr == Buffer::GetName(buff))
		{
			continue;
		}

		if (strstr(Buffer::GetName(buff), "gemarker") == NULL)
		{
			continue;
		}

		auto BufferCount = Buffer::GetBufferCount(buff);
		if (0 == BufferCount)
		{
			BufferCount = 1;
		}

		auto ske = Player::GetSkillByIndex(2);
		if (!ske)
		{
			return false;
		}

		if (!ske->BCoolDown() || ske->GetLevel() < 1 || Player::GetCurrMp() < ske->GetExpendMp())
		{
			return false;
		}

		//���㻤��
		auto defense = Monster::GetDefensePower(mons) - Player::GetArmour();
		if (defense < 0)
		{
			defense = 0;
		}
		//�����˺����㹫ʽ (�����˺� + �������ӳɱ���) * ��100.0f / (100.0f + ������*��1.0 - ��͸�ʣ�)����
		//auto hurt =  Player::GetAttackPower() * (100.0f / (100.0f + (defense * (1.0f - Player::GetArmourPenetration()))); //���˱���
		auto AttackHurt = Player::GetAttackPower(); //���˱���
		auto AllHurt = ske->GetSkillAggressivity() + 0.6f * AttackHurt + ((6.0f + (float)ske->GetLevel() * 4.0f) + AttackHurt * (0.2f + (float)ske->GetLevel() * 0.025f))*((float)(BufferCount - 1)) - 1;
		//������˺���˺�
		auto DestHurt = AllHurt * (100.0f / (100.0f + (defense * (1.0f - Player::GetArmourPenetration()))));
		if (DestHurt > Monster::GetCurrentHealth(mons) &&
			Monster::GetCurrentHealth(mons) > 1.0f &&
			Monster::BVisable(mons) && 
			!Monster::BDead(mons)){
			if (UseSkillToMons(mons, 2, 0, 0, false)){
				return true;
			}
		}
	}

	return false;
}







//Player::Player()
//{
//	m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
//	if (m_playerObj)
//	{
//		m_skService = std::shared_ptr<CSkillService>(new CSkillService(m_playerObj));
//		m_skService->travse();
//		m_buffService = std::shared_ptr<CBufferService>(new CBufferService(m_playerObj));
//		m_buffService->travse();
//	}
//
//	m_ZouKanTimeSec[0] = 0.0f;
//	m_ZouKanTimeSec[1] = 0.0f;
//}



char * Player::GetName()
{
	
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetName(m_playerObj);
	}
	return nullptr;
}

char * Player::GetHeroName()
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetHeroName(m_playerObj);
	}
	return nullptr;
}

float Player::GetCurrHp()
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetCurrentHealth(m_playerObj);
	}
	return 0.0f;
}

float Player::GetMaxHp()
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetMaxHealth(m_playerObj);
	}
	return 0.0f;
}

float Player::GetCurrMp()
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetCurrentMagic(m_playerObj);
	}
	return 0.0f;
}

float Player::GetMaxMp()
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetMaxMagic(m_playerObj);
	}
	return 0.0f;
}

PEP_POINT_3D Player::GetCurrPostion() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetCurrentPos(m_playerObj);
	}
	return nullptr;
}

bool Player::BDead() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::BDead(m_playerObj);
	}
	return true;
}

bool Player::BVisableSee() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::BVisable(m_playerObj);
	}
	return false;
}

bool Player::BMoving() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::BMoving(m_playerObj);
	}
	return false;
}

PEP_POINT_3D Player::GetOrientation() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetOrientation(m_playerObj);
	}
	return nullptr;
}

DWORD Player::GetCamp() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetCamp(m_playerObj);
	}
	return 0;
}

DWORD Player::GetType() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetType(m_playerObj);
	}
	return 0;
}

DWORD Player::GetLevel() 
{
	return 0;
}

float Player::GetAttackPower() 
{
	auto dwBase = utils::read<DWORD>(Base_UIAddr);
	if (dwBase)
	{
		auto dwOffset1 = utils::read<DWORD>(dwBase + 0x28);
		if (dwOffset1)
		{
			auto dwOffset2 = utils::read<DWORD>(dwOffset1 + 0x4);
			if (dwOffset2)
			{
				auto dwOffset3 = utils::read<DWORD>(dwOffset2 + 0x10);
				if (dwOffset3)
				{
					return  utils::read<float>(dwOffset3 + 0x10);
				}
			}
		}
	}
	return 0.0f;
}

float Player::GetMoveSpeed() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetMoveSpeed(m_playerObj);
	}
	return 0;
}

float Player::GetAttackSpeed() 
{
	auto dwBase = utils::read<DWORD>(Base_UIAddr);
	if (dwBase)
	{
		auto dwOffset1 = utils::read<DWORD>(dwBase + 0x28);
		if (dwOffset1)
		{
			auto dwOffset2 = utils::read<DWORD>(dwOffset1 + 0x4);
			if (dwOffset2)
			{
				auto dwOffset3 = utils::read<DWORD>(dwOffset2 + 0x10);
				if (dwOffset3)
				{
					auto Speed = utils::read<float>(dwOffset3 + 0x18);
					if (Speed)
					{
						return (float)(1.0 / Speed);
					}
				}
			}
		}
	}
	return 0.0f;
}

float Player::GetAttackDistance() 
{
	auto dwBase = utils::read<DWORD>(Base_UIAddr);
	if (dwBase)
	{
		auto dwOffset1 = utils::read<DWORD>(dwBase + 0x28);
		if (dwOffset1)
		{
			auto dwOffset2 = utils::read<DWORD>(dwOffset1 + 0x4);
			if (dwOffset2)
			{
				auto dwOffset3 = utils::read<DWORD>(dwOffset2 + 0x10);
				if (dwOffset3)
				{
					return utils::read<float>(dwOffset3 + 0x68);

				}
			}
		}
	}
	return 0.0f;
}

Skill* Player::GetSkillByIndex(DWORD index)
{
	if (!m_skService)
	{
		auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
		if (m_playerObj)
		{
			m_skService = std::shared_ptr<CSkillService>(new CSkillService(m_playerObj));
			m_skService->travse();
		}
	}
	return m_skService->GetSkillByIndex(index);
}



float Player::GetArmour() 
{
	auto dwBase = utils::read<DWORD>(Base_UIAddr);
	if (dwBase)
	{
		auto dwOffset1 = utils::read<DWORD>(dwBase + 0x28);
		if (dwOffset1)
		{
			auto dwOffset2 = utils::read<DWORD>(dwOffset1 + 0x4);
			if (dwOffset2)
			{
				auto dwOffset3 = utils::read<DWORD>(dwOffset2 + 0x10);
				if (dwOffset3)
				{
					return utils::read<float>(dwOffset3 + 0x58);
				}
			}
		}
	}
	return 0.0f;
}

float Player::GetArmourPenetration() 
{
	auto dwBase = utils::read<DWORD>(Base_UIAddr);
	if (dwBase)
	{
		auto dwOffset1 = utils::read<DWORD>(dwBase + 0x28);
		if (dwOffset1)
		{
			auto dwOffset2 = utils::read<DWORD>(dwOffset1 + 0x4);
			if (dwOffset2)
			{
				auto dwOffset3 = utils::read<DWORD>(dwOffset2 + 0x10);
				if (dwOffset3)
				{
					return utils::read<float>(dwOffset3 + 0x5c) / 100.0f;
				}
			}
		}
	}
	return 0.0f;
}

float Player::GetMonsModelSize() 
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetModelWidth(m_playerObj);
	}
	return 0.0f;
}

float Player::GetSpellPower() 
{
	auto dwBase = utils::read<DWORD>(Base_UIAddr);
	if (dwBase)
	{
		auto dwOffset1 = utils::read<DWORD>(dwBase + 0x28);
		if (dwOffset1)
		{
			auto dwOffset2 = utils::read<DWORD>(dwOffset1 + 0x4);
			if (dwOffset2)
			{
				auto dwOffset3 = utils::read<DWORD>(dwOffset2 + 0x10);
				if (dwOffset3)
				{
					return utils::read<float>(dwOffset3 + 0x0);
				}
			}
		}
	}
	return 0.0f;
}

float Player::GetXerathPowerTime() 
{
	auto skObj = GetSkillByIndex(0);
	if (skObj)
	{
		return skObj->GetXerathPowerTime();
	}
	return 0.0f;
}

float Player::GetXerathSkillCurrDistance()
{
	float dis = 0.0f;
	auto time = Player::GetXerathPowerTime();
	if (time > 0) {
		auto tempTime = GameCall::GetGameTickTime() - time;
		dis = tempTime * 450.0f - 100.0f;

		if (dis > 700.0f) {
			dis = 700.0f;
		}
	}

	return (700.0f + dis);
}

DWORD Player::GetObjectForSkill()
{
	auto m_playerObj = utils::read<DWORD>(Base_RoleSelfAddr);
	if (m_playerObj)
	{
		return Monster::GetObjectForSkill(m_playerObj);
	}
	return 0;
}

EP_POINT_3D Player::ClacAnticipationPnt(DWORD mons, float skDelay, float MissileSpeed)
{
	EP_POINT_3D pnt = { 0.0f };
	if (Monster::GetType(mons) != 0x4) {
		memcpy(&pnt, Monster::GetCurrentPos(mons), sizeof(EP_POINT_3D));
		return pnt;
	}

	//�������Ԥ�е���жϣ�����ɼ���2����͵�ǰ������һ�����򣬾Ͳ���Ĭ��Ԥ�е㡣����Ԥ�о��� /2 
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	int oriChangleNum = 0;
	float maxDis = 0.0f;
	for (auto &temp : menu->monsList)
	{
		if (temp.obj == mons)
		{
			//1.����Ԥ�еĳ���
			auto curOri = Monster::GetOrientation(mons);
			auto curAngles = atan2(curOri->y, curOri->x);

			for (auto i = 0; i != temp.LastOrientation.size(); i++)
			{
				if (i == temp.LastOrientation.size() - 1)
				{
					auto ori = temp.LastOrientation.at(i);
					auto angles = atan2(ori.y, ori.x);
					auto curOri = Monster::GetOrientation(mons);
					auto curAngles = atan2(curOri->y, curOri->x);

					if (abs(curAngles - angles) < (PI / 2.0F))
					{
						oriChangleNum++;
					}
					else {
						oriChangleNum = 0;
					}

				}
				else {
					auto ori = temp.LastOrientation.at(i);
					auto angles = atan2(ori.y, ori.x);
					auto NextOri = temp.LastOrientation.at(i + 1);
					auto curAngles = atan2(NextOri.y, NextOri.x);

					if (abs(curAngles - angles) < (PI / 2.0F))
					{
						oriChangleNum++;
					}
					else {
						oriChangleNum = 0;
					}
				}
				
			}

			//2.����λ�Ƶľ���
			for (auto &pos : temp.LastPosition)
			{
				auto dis = sqrt(utils::GetDistance(&pos, Monster::GetCurrentPos(mons)));
				if (dis > maxDis) {
					maxDis = dis;
				}
			}
			break;
		}
	}



	if (Monster::BMoving(mons))
	{
		//��ȡ���ӳٺ������
		float delay = 0.0f;
		if (MissileSpeed > 0)
		{
			delay = skDelay + sqrt(utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(mons))) / MissileSpeed;
		}
		else
		{
			delay = skDelay;
		}

		float disMonsMove = 0.0f;
		float RealDelay = (float)oriChangleNum * 0.1f;
		if (RealDelay > delay) {

			//����������߶�����ô��ȡ�ƶ��������벢�ҳ�2��ΪԤ�о���

			//if (maxDis > 0) {
			//	disMonsMove = maxDis *tempDelay;
			//}
			//else {
				disMonsMove = Monster::GetMoveSpeed(mons) * delay;
			//}

#ifdef _USER_DBG
				utils::log("HXL: Ŀ����һ�������ߣ�oriChangleNum=%d skDelay=%f disMonsMove=%f", oriChangleNum, delay, disMonsMove);
#endif // 
		}
		else {
			if (0 == oriChangleNum) {
				disMonsMove = Monster::GetMoveSpeed(mons) * 0.1f;
			}
			else {
				//����������߶�����ô��ȡ�ƶ��������벢�ҳ�2��ΪԤ�о���
				//if (maxDis > 0) {
				//	disMonsMove = maxDis *tempDelay;
				//}
				//else {
				disMonsMove = Monster::GetMoveSpeed(mons) * delay * 0.5f;
				//}
			}

#ifdef _USER_DBG
			utils::log("HXL: Ŀ�������߶���oriChangleNum=%d skDelay=%f disMonsMove=%f", oriChangleNum, delay, disMonsMove);
#endif // 
		}
		
		if (Monster::BMoving(mons)){
			auto angles = atan2(Monster::GetOrientation(mons)->y, Monster::GetOrientation(mons)->x);
			pnt.x = Monster::GetCurrentPos(mons)->x + cos(angles) * disMonsMove;
			pnt.z = Monster::GetCurrentPos(mons)->z;
			pnt.y = Monster::GetCurrentPos(mons)->y + sin(angles) * disMonsMove;
		}
		else {
			memcpy(&pnt, Monster::GetCurrentPos(mons), sizeof(EP_POINT_3D));
		}

	}
	else {
		memcpy(&pnt, Monster::GetCurrentPos(mons), sizeof(EP_POINT_3D));
	}

	return pnt;
}
