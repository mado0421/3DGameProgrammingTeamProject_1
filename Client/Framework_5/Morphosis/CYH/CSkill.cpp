#include "stdafx.h"
#include "CSkill.h"


CSkill::CSkill(BYTE main, BYTE sub1, BYTE sub2, XMFLOAT4X4& world, XMFLOAT3& vector)
{
	//타이밍 넣어야됨
	switch (main)
	{
	case grenade:
		m_missileType = new CSkillJudge(0, world, vector, 5, INFINITE, linear, range);
		m_mainEffect.addEffect(mainType, grenade);
		break;
	case poison:
		m_missileType = new CSkillJudge(0, world, vector, 0, INFINITE, target, none);
		m_mainEffect.addEffect(mainType, poison);
		break;
	case shield:
		m_missileType = new CSkillJudge(0, world, vector, 0, INFINITE, construct, none);
		m_mainEffect.addEffect(mainType, shield);
		break;
	}
	if (main == grenade)
	{
		switch (sub1)
		{
		case poison:
			m_subEffect[0].addEffect(subActive, poison);
			m_Timing_takeEffect = true;
			break;					 
		case shield:				 
			m_subEffect[0].addEffect(subActive, shield);
			m_Timing_takeEffect = true;
			break;
		}
		switch (sub2)
		{
		case poison:
			m_subEffect[1].addEffect(subActive, poison);
			m_Timing_takeEffect = true;
			break;
		case shield:
			m_subEffect[1].addEffect(subActive, shield);
			m_Timing_takeEffect = true;
			break;
		}
	}
	else
	{
		switch (sub1)
		{
		case grenade:
			m_subEffect[0].addEffect(subPassive, grenade);
			m_Timing_takeEffect = true;
			break;
		case poison:
			m_subEffect[0].addEffect(subPassive, poison);
			m_Timing_use = true;
			break;
		case shield:
			m_subEffect[0].addEffect(subPassive, shield);
			m_Timing_use = true;
			break;
		}
		switch (sub2)
		{
		case grenade:
			m_subEffect[1].addEffect(subPassive, grenade);
			m_Timing_takeEffect = true;
			break;
		case poison:
			m_subEffect[1].addEffect(subPassive, poison);
			m_Timing_use = true;
			break;
		case shield:
			m_subEffect[1].addEffect(subPassive, shield);
			m_Timing_use = true;
			break;
		}
	}
}


CSkill::~CSkill()
{
}

void CSkill::OnUse()
{

}
