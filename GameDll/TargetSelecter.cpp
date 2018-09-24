#include "stdafx.h"
#include "TargetSelecter.h"
#include "Monster.h"
#include "utils.h"
#include "baseaddr.h"
#include "Player.h"

//������
#ifdef _USER_DBG
#include "GameMenu.h"
#endif // _USER_DBG

 CTargetSelecter* CTargetSelecter::m_pInstance = nullptr;
 std::mutex CTargetSelecter::m_mutex;

 //��������
 std::vector<DWORD> g_personList;
 std::vector<DWORD> g_monsterList;
 std::mutex g_mutex;
 HANDLE g_evt = { 0 };
CTargetSelecter::CTargetSelecter()
{
	g_evt = CreateEvent(NULL, FALSE, FALSE, NULL);
}


CTargetSelecter::~CTargetSelecter()
{
}

CTargetSelecter * CTargetSelecter::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CTargetSelecter();
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

bool CTargetSelecter::travse()
{
	m_personList.clear();
	m_monsterList.clear();

	auto startAddr = utils::read<DWORD>(Base_MonsterArrayAddr);
	auto endAddr = utils::read<DWORD>(Base_MonsterArrayAddr + 0x4);

	if (!startAddr || !endAddr)
	{
		return false;
	}

	for (auto i = startAddr; i <= endAddr; i += 4)
	{
		auto temp = utils::read<DWORD>(i);
		if (!temp) {
			continue;
		}

		if (Monster::GetCamp(temp) != Player::GetCamp())
		{
			//��������Ѿ�����������Ҷ���
			if (Monster::GetType(temp) == 0x4) 
			{
				m_personList.push_back(temp);
			}
			//�����С��
			else if (Monster::GetType(temp) == 0x6) 
			{

				//����δ֪�Ķ���
				auto MaxHp = Monster::GetMaxHealth(temp);
				if (MaxHp< 1001.0f && MaxHp > 999.0f)
				{
					continue;
				}

				if (MaxHp < 101.0f && MaxHp > 99.0f)
				{
					continue;
				}

				//���˺�г���������ʬ��
				if (MaxHp < 77.0f && MaxHp > 75.0f)
				{
					continue;
				}


				if (MaxHp < 10.0f && MaxHp > 0.0f)
				{
					continue;
				}


				if (utils::GetDistance(Monster::GetCurrentPos(temp), Player::GetCurrPostion()) < 2000.0f * 2000.0f &&
					!Monster::BDead(temp) &&
					Monster::BVisable(temp))
				{
					m_monsterList.push_back(temp);
				}
			}
			else
			{
#ifdef _USER_DBG
				//auto menu = CGameMenu::GetInstance()->GetGameMenu();
				//if (menu->bFarming)
				//{
				//	if (utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(temp)) < 1500.0f * 1500.0f)
				//	{
				//		utils::log("%x , %s , (%f,%f), %x , %x",
				//			temp,
				//			Monster::GetName(temp),
				//			Monster::GetCurrentHealth(temp),
				//			Monster::GetMaxHealth(temp),
				//			Monster::GetType(temp),
				//			Monster::GetCamp(temp));
				//	}
				//}

#endif // _USER_DBG
			}


		}
	}

	return true;
}



void CTargetSelecter::swapPersonList()
{
	g_mutex.lock();
	g_personList.swap(m_personList);
	g_mutex.unlock();
}

void CTargetSelecter::swapMonsterList()
{
	g_mutex.lock();
	g_monsterList.swap(m_monsterList);
	g_mutex.unlock();
}

std::vector<DWORD>& CTargetSelecter::GetPersonList()
{
	// TODO: �ڴ˴����� return ���
	return m_personList;
}


DWORD GetHealthLowerestPerson(std::vector<DWORD>& list, float dis)
{
	if (dis <= 0)
	{
		return 0;
	}
	float minHealth = FLT_MAX;
	DWORD obj = 0;

	for (auto &temp : list)
	{
		if (!temp)
		{
			continue;
		}
		auto disTemp = dis + Monster::GetModelWidth(temp) * 1.5f;
		if (Monster::BVisable(temp) && 
			!Monster::BDead(temp) &&
			utils::GetDistance(Monster::GetCurrentPos(temp), Player::GetCurrPostion()) < disTemp  * disTemp)
		{
			auto currentHealth = Monster::GetCurrentHealth(temp);
			if (currentHealth < minHealth)
			{
				minHealth = currentHealth;
				obj = temp;
			}
		}
	}

	return obj;
}

DWORD GetCassiopeiaPoisonPerson(std::vector<DWORD>& list, float dis)
{

	if (dis <= 0)
	{
		return 0;
	}
	float minHealth = FLT_MAX;
	DWORD obj = 0;

	float PoisonHealth = FLT_MAX;
	DWORD poisonObj = 0;

	for (auto temp : list)
	{
		if (!temp)
		{
			continue;
		}
		auto disTemp = dis + Monster::GetModelWidth(temp) * 1.5f;
		if (Monster::BVisable(temp) &&
			!Monster::BDead(temp) &&
			utils::GetDistance(Monster::GetCurrentPos(temp), Player::GetCurrPostion()) < disTemp  * disTemp)
		{
			auto currentHealth = Monster::GetCurrentHealth(temp);
			if (currentHealth< minHealth)
			{
				minHealth = currentHealth;
				obj = temp;
			}


			//��ȡ����buff
			CBufferService cbf(temp);
			for (auto obj : cbf.GetBufferList())
			{
				if (!Buffer::GetName(obj))
				{
					continue;
				}

				if (strstr(Buffer::GetName(obj), "assiopeia") != NULL)
				{
					//˵�������Ѿ��ж���
					if (currentHealth < PoisonHealth)
					{
						PoisonHealth = currentHealth;
						poisonObj = temp;
					}
				}
			}
		}
	}

	if (poisonObj)
	{
		return poisonObj;
	}

	return obj;

}

bool IsCoveredByMonster(std::vector<DWORD>& list, DWORD mons, float dis, float width, float delay, float MissileSpeed)
{
	//Ԥ������
	auto pnt = Player::ClacAnticipationPnt(mons, delay, MissileSpeed);

	//������ҵ�Ŀ��ĽǶ�
	//float anglesTarget = atan2(Monster::GetCurrentPos(mons)->y - Player::GetCurrPostion()->y, Monster::GetCurrentPos(mons)->x - Player::GetCurrPostion()->x);
	float anglesTarget = atan2(pnt.y - Player::GetCurrPostion()->y, pnt.x - Player::GetCurrPostion()->x);
	if (anglesTarget < 0)
	{
		anglesTarget = PI * 2.0f + anglesTarget;
	}
	//������ҵ�Ŀ��ľ���
	auto dis2Player = sqrt(utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(mons)));
	auto monsWidth = Monster::GetModelWidth(mons);

	//��ȡ��С���б�
	for (auto &temp : list)
	{
		//���С������������С�����ɼ����ͼ�������һ��С��,˵��û���ڵ�
		if (Monster::BDead(temp) || !Monster::BVisable(temp))
		{
			continue;
		}

		//����С������ҵ�Բ�뾶
		auto disMons2Player = utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(temp));
		//1.���С���ڼ��ܷ�Χ�⣬��û���ڵ�
		auto modelWidth = Monster::GetModelWidth(temp);
		if (disMons2Player > (dis + modelWidth)* (dis + modelWidth))
		{
			continue;
		}

		//2.���С������ҵľ��� > (��ҵ����˵þ��� + ����ģ�ʹ�С��,˵��û���ڵ���
		if (disMons2Player > (dis2Player + monsWidth)* (dis2Player + monsWidth))
		{
			continue;
		}

		//����С���Ļ��ȽǴ�С,���ҽ��Ƕ�ת��Ϊ 0-2PI֮��
		float angles = atan2(Monster::GetCurrentPos(temp)->y - Player::GetCurrPostion()->y, Monster::GetCurrentPos(temp)->x - Player::GetCurrPostion()->x);
		if (angles < 0)
		{
			angles = PI * 2.0f + angles;
		}
		//����С���ͼ���֮��ļн�,�������ֵ > 90�ȣ�ֱ�ӷ���
		auto destAngles = abs(anglesTarget - angles);
		if (destAngles > (PI / 2.0f))
		{
			continue;
		}

		//���ݼн����߾���
		auto dis2Pnt = sin(destAngles) * (sqrt(disMons2Player));

		//���С�����ĵ��λ�� �� �������Ĵ��ߵľ��� < ���ܿ�� + ����뾶��˵��������ײ
		if (dis2Pnt <= (modelWidth + width))
		{
			return true;
		}

	}

	return false;
}

DWORD GetDangerousMostPerson(std::vector<MONSTER_OBJECT>& list, float dis, DWORD percent)
{
	if (dis <= 0)
	{
		return 0;
	}

	//����Ѫ����� & �������õ�Ѫ�������Ĺ���
	float minHealth = FLT_MAX;
	DWORD HealthMostLowObj = 0;

	//����Σ�յȼ���ߵĹ���
	DWORD Dangerousobj = 0;
	int iDangerous = -1;
	for (auto &temp : list)
	{
		if (!temp.obj)
		{
			continue;
		}
		//�жϹ�������Ƿ��ڼ��ܷ�Χ��
		auto disTemp = dis + Monster::GetModelWidth(temp.obj) * 1.5f;
		if (Monster::BVisable(temp.obj) &&
			!Monster::BDead(temp.obj) &&
			utils::GetDistance(Monster::GetCurrentPos(temp.obj), Player::GetCurrPostion()) < disTemp  * disTemp)
		{

			auto curHealth = Monster::GetCurrentHealth(temp.obj);

			//����Ѫ����͵Ĺ���
			if (curHealth < minHealth)
			{
				minHealth = curHealth;
				HealthMostLowObj = temp.obj;
			}

			//����Σ�յȼ���ߵĹ���
			if (temp.dangerous > iDangerous)
			{
				iDangerous = temp.dangerous;
				Dangerousobj = temp.obj;
			}

			//���Σ�յȼ� == ��ǰ���Σ�ռ�
			if (temp.dangerous == iDangerous)
			{
				if (Monster::GetCurrentHealth(temp.obj) < Monster::GetCurrentHealth(Dangerousobj))
				{
					iDangerous = temp.dangerous;
					Dangerousobj = temp.obj;
				}
			}
		}
	}

	//�ж�Ѫ����͵Ĺ����Ƿ�������õ���Ͱٷֱ�
	auto curHealth = Monster::GetCurrentHealth(HealthMostLowObj);
	auto maxHealth = Monster::GetMaxHealth(HealthMostLowObj);
	if (maxHealth > 0)
	{
		auto curPercent = curHealth / maxHealth;
		if (curPercent < (percent / 100.0f))
		{
			return HealthMostLowObj;
		}
	}

	return Dangerousobj;
}
