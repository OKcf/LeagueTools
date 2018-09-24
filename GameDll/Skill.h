#pragma once

enum SK_SKILL_TYPE
{
	SK_SKILL_LINE = 0x009529A0,   //ָ���Լ��ܣ�����Q
	SK_SKILL_LOCK = 0x00959E00,   //�������ܣ����簲��Q ��ŮE
	SK_SKILL_SECTOR = 0x00952780, //���μ��ܣ����簲��W,��ŮR
	SK_SKILL_ROUND = 0x00959C40, //Բ�μ��ܣ�����R ,����R
	SK_SKILL_ROUND2 = 0x00959C70, //����˹R , ����Q
	SK_SKILL_TOSELF = 0x00959DD0, //���Լ��ͷŵļ���
	SK_SKILL_VIKTOR = 0x00952C70, //ά���е�E
};

class Skill
{
public:
	Skill(DWORD dwNodeBase, DWORD _index);
	~Skill();
	//��ȡ��������
	DWORD GetIndex() const;
	//��ȡ��������
	char* GetName() const;
	//��ȡ���ܵȼ�
	DWORD GetLevel() const;
	//�����Ƿ���ȴ
	bool BCoolDown() const;
	//��ȡ�������ĵ�����
	float GetExpendMp() const;
	//��ȡ���ܷ�Χ
	float GetDistance() ;
	//��ȡ���ܷ�Χ2
	float GetDistance1() ;
	//��ȡ���ܷ�Χ2
	float GetDistance2() ;
	//��ȡ���ܿ��
	float GetBreadth() ;
	//��ȡ���ܹ�����
	float GetSkillAggressivity() const;
	//��ȡ���ܹ������ӳɱ���
	//��ȡ��������
	DWORD GetSkillType();
	//��ȡ��ŮE���ܻ����˺�
	float GetBaseAggressivity() const;
	//��ȡ����˹����ʱ��
	float GetXerathPowerTime();
	DWORD GetNodeBase();

	//���ü��ܷ�Χ
	void SetDistance(float dis);


	//���ü��ܿ��
	void SetWidth(float width);
	float GetWidth();

	//�Ƿ��ж��ڵ�
	void SetJudgmentCover(bool _b);
	bool GetJudgmentCover();
private:
	DWORD m_index;
	WORD m_skillType;
	DWORD m_dwNodeBase;
	float m_Distance;
	//���ܷ�Χ
	float m_skDistance1[7];
	float m_skDistance2[7];
	float m_skBreadth[7];
	//���ܹ�����
	float m_skAggressivity[7];
	//���ܿ��
	float m_skWidth;
	//�Ƿ��жϼ����ڵ�
	bool m_bJudgmentCover;
};