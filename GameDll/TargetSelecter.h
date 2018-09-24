#pragma once
#include <mutex>
#include <vector>
#include "Monster.h"
#include "GameMenu.h"
extern std::vector<DWORD> g_personList;
extern std::vector<DWORD> g_monsterList;
extern std::mutex g_mutex;
extern std::vector<DWORD> g_personList2;
extern  HANDLE g_evt;


class CTargetSelecter
{
	CTargetSelecter();
	CTargetSelecter(const CTargetSelecter&) = delete;
	void operator= (const CTargetSelecter&) = delete;
	static CTargetSelecter* m_pInstance;
	static std::mutex m_mutex;
	std::vector<DWORD> m_personList;
	std::vector<DWORD> m_monsterList;
public:
	~CTargetSelecter();
	static CTargetSelecter* GetInstance();

	bool travse();
	void swapPersonList();
	void swapMonsterList();
	std::vector<DWORD>& GetPersonList();
};

//��ȡѪ����͵Ĺ���
DWORD GetHealthLowerestPerson(std::vector<DWORD>& list, float dis);
//��ȡ��Ů�ж��Ĺ���
DWORD GetCassiopeiaPoisonPerson(std::vector<DWORD> &list, float dis);
//�ж��Ƿ���С���ڵ� (С���б� Ŀ������ , ���ܷ�Χ)
bool IsCoveredByMonster(std::vector<DWORD>& list, DWORD mons , float dis, float width,float delay, float misSpeed);

//��ȡΣ�յȼ���ߵĹ���
DWORD GetDangerousMostPerson(std::vector<MONSTER_OBJECT>& list, float dis, DWORD percent);