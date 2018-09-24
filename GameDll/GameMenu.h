#pragma once
#include "stdafx.h"

#include <mutex>
#include <list>
#include <vector>
#include <deque>
typedef struct _MONSTER_OBJECT
{
	_MONSTER_OBJECT() : obj(0) , dangerous(3), name(""), heroname(""),_sid(""), _format(""){}
	DWORD obj; //�������
	int dangerous; //Σ�յȼ�
	std::string name;//�������
	std::string heroname; //Ӣ������
	std::string _sid;
	std::string _format;
	std::deque<EP_POINT_3D> LastOrientation;  //����
	std::deque<EP_POINT_3D> LastPosition;  //λ�ù켣
}MONSTER_OBJECT, *PMONSTER_OBJECT;


typedef struct _GAME_MENU
{
	//����̧���ӳ�
	float skDelay[4];
	//�����ٶ�
	float skSpeed[4];
	//�Ƿ�������
	bool bFollowMouse;
	//�Ƿ�ƽA
	bool bAttack;
	//�Ƿ�����߿�
	bool bRenderAttackDis;
	//����
	DWORD dwComboHotKey;
	//��������QWER
	bool bLockSk[4];
	//����ʹ�ü���
	bool bComboSk[4];
	//����˳��
	int iComboSequence[4];
	//�Ƿ񲹵�
	bool bFarming;
	//�����ȼ�
	DWORD dwFarmingHotkey;
	//���ʹ��ƽA
	bool bQingArmyWithAttack;
	//���ʹ�ü���
	bool bQingArmyWithSk[4];
	//����ȼ�
	DWORD dwQingArmyHotkey;
	//Ŀ��ѡ����
	std::vector<MONSTER_OBJECT> monsList;
	//���ȹ���Ѫ�����ڱ�����
	int dwTargetPercent;
	//�����ȼ��Ƿ���
	bool bComboHotKeyDown;
	//����ȼ��Ƿ���
	bool bQingArmyHotkeyDown;
	//�����ȼ��Ƿ���
	bool bFarmingHotkeyDown;
}GAME_MENU, *PGAME_MENU;

class CGameMenu
{
	CGameMenu();
	CGameMenu(const CGameMenu&) = delete;
	CGameMenu(const CGameMenu&&) = delete;
	void operator= (const CGameMenu&) = delete;
	void operator= (const CGameMenu&&) = delete;

	static std::mutex m_mutex;
	static CGameMenu* m_pInstance;
public:
	~CGameMenu();
	static CGameMenu* GetInstance();
	static void DestoryInstance();
	GAME_MENU* GetGameMenu();
	void SetMonsterList(std::vector<DWORD> list);
	bool GetComboKeyState();
	bool GetFarmingKeyState();
	bool GetQingArmyKeyState();
	void UpdateOrientation(); //��������ĳ���
private:
	GAME_MENU m_gameMenu;
};

