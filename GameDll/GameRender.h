#pragma once
#include <mutex>

#include "d3d9.h"
#include "d3dx9.h"
typedef long(__stdcall * pReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef long(__stdcall * pPresent)(IDirect3DDevice9 * device, THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

class CGameRender 
{
	CGameRender();
	CGameRender(const CGameRender&) = delete;
	void operator= (const CGameRender&) = delete;
	CGameRender(const CGameRender&&) = delete;
	void operator= (const CGameRender&&) = delete;

	static CGameRender* m_pInstance;
	static std::mutex m_mutex;

public:
	~CGameRender();
	static CGameRender* GetInstance();
	static void DestoryInstance();
	//��ʼ������
	void Render();
	bool Initialize();
	bool& BVisiable();
	LONG_PTR& GetOldWndProc();
	PVOID GetOldReset();
	PVOID GetOldPresent();
	static LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	void SetBVisiable();
private:
	void RenderTips(); //������ʾ��
	void RenderSkConfig(); //��Ⱦ�����ӳ�
	void RenderMoveAndAttack(); //��Ⱦ�߿�
	void RenderLockSkill(); //��Ⱦ��������
	void RenderCombo(); //��Ⱦ����
	void RenderFarming(); //��Ⱦ����
	void RenderQingArmy(); //��Ⱦ���
	void RenderTargetSelecter(); //��ȾĿ��ѡ����
	bool UseSkillByIndex(DWORD index);
private:
	HWND m_leagueHwnd; 
	LONG_PTR m_ulOldWndProc;
	bool m_bVisiable;
	PVOID m_oldReset;
	PVOID m_oldPresent;

	std::string m_sTipsText;
	std::string m_skConfigText; //��������
	std::string m_skDelayText; //����ǰҡ�ӳ�
	std::string m_skSpeedText; //�����ٶ�
	std::string m_sMoveAndAttackText; //�߿�����
	std::string m_sFollowMosueText; //�������
	std::string m_sAttackText; //��ͨ����
	std::string m_sRenderAttackDisText; //�����չ�����
	std::string m_sComboHotkeyText; //�����ȼ�
	std::string m_sLockSkillText;  //��������
	std::string m_sComboText; //��������
	std::string m_sFarmingText; //��������
	std::string m_sOpenFramingText; //��������
	std::string m_sFarmingHotkeyText; //�����ȼ�
	std::string m_sQingArmyText; //�������
	std::string m_sQingArmtAttackText; //�չ����
	std::string m_sQingArmyHotkey; //����ȼ�
	std::string m_sTargetSelecterText; //Ŀ��ѡ��������
	std::string m_sTargetMinPercentText; //Ŀ�����Ѫ��ѡ��
	std::string m_sCombosk1Text; //����ʹ�ü���1
	std::string m_sCombosk2Text; //����ʹ�ü���2
	std::string m_sCombosk3Text; //����ʹ�ü���3
	std::string m_sCombosk4Text; //����ʹ�ü���4
};

/*
// API hook ����
*/
long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
long __stdcall hkPresent(IDirect3DDevice9* device, THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
