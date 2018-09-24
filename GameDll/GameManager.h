#pragma once
#include <mutex>
#include <vector>
class CGameManager
{
	CGameManager();
	CGameManager(const CGameManager&) = delete;
	void operator= (const CGameManager&) = delete;
	CGameManager(const CGameManager&&) = delete;
	void operator= (const CGameManager&&) = delete;

	static CGameManager* m_pInstance;
	static std::mutex m_mutex;

public:
	~CGameManager();
	static CGameManager* GetInstance();
	static void DestoryInstance();

	bool Initialize();
	void setExitTag(bool _b);

	//ͨ�������߳�
	void GeneralComboThread();
	//Ŀ��ѡ�����߳�
	void TargetSelecterThread();
	//��Ů�����߳�
	void CassiopeiaComboThread();
	//�������������߳�
	void TristanaComboThread();
	//��Ԩ�޿������߳�
	void KogMawComboThread();
	//����Ь�����߳�
	void KalistaComboThread();
	//������������߳�
	void EzrealComboThread();
	//����˹�����߳�
	void XerathComboThread();
	//ά���������߳�
	void ViktorComboThread();
private:
	//ͨ���߿�
	void GeneralZouKan();
	//����
	void Farming();
	//���
	void QingArmy();
	//ʹ�ü���1
	bool UseSkill1();
	//ʹ�ü���2
	bool UseSkill2();
	//ʹ�ü���3
	bool UseSkill3();
	//ʹ�ü���4
	bool UseSkill4();
	//��ȡΣ�յȼ���ߵĵ�����Ҳ���ʹ�ü���
	bool UseSkillByIndexWithDangerous(DWORD index);
	//��ȡѪ����͵�С������ʹ�ü���
	bool UseSkill2MonsterHealthLowestByIndex(DWORD index);
private:
	std::thread m_targetSelecterTh;
	std::thread m_ComboSelecterTh;
	bool m_bExitGame;
	std::vector<DWORD> m_monsterList;
	std::vector<DWORD> m_personList;
	float m_skBeginDaley; //����̧��ʱ��
};

