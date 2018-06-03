#pragma once
#include "CSkillEffect.h"
#include "CSkillJudge.h"
struct SKILLNUMBER
{
	BYTE main;
	BYTE sub1;
	BYTE sub2;
};

class CSkill
{
public:
	CSkillJudge* m_missileType;

	CSkillEffect m_mainEffect;
	CSkillEffect m_subEffect[2];

	//int m_skillID;

	bool m_Timing_use= false;
	bool m_Timing_cast= false;
	bool m_Timing_fire= false;
	bool m_Timing_update= false;
	bool m_Timing_takeEffect= false;
	//bool m_Timing_duration; 이 부분은 캐릭터에 takeEffect에서 처리
	bool m_Timing_destroy;


	CSkill(BYTE main, BYTE sub1, BYTE sub2, XMFLOAT4X4& world, XMFLOAT3& vector);
	~CSkill();

	void OnUse();
};

