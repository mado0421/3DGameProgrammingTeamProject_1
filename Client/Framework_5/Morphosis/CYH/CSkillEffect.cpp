#include "stdafx.h"
#include "CSkillEffect.h"


CSkillEffect::CSkillEffect()
{
	
}


CSkillEffect::~CSkillEffect()
{

}

bool CSkillEffect::addEffect(BYTE addType, BYTE kind)
{
	int itemp;
	float ftemp;
	switch (kind)
	{
	case grenade:
		switch (addType)
		{
		case mainType:
			itemp = 4;
			m_effect.push_back(new Effect(e_damage, &itemp, sizeof(int)));
			break;
		case subActive:
			ftemp = 1.2;
			m_effect.push_back(new Effect(e_increaseRange, &ftemp, sizeof(float)));
			break;
		case subPassive:
			itemp = 2;
			m_effect.push_back(new Effect(e_damage, &itemp, sizeof(int)));
			break;
		}
		break;
	case poison:
		switch (addType)
		{
		case mainType:
			ftemp = 2.0;
			m_effect.push_back(new Effect(e_dotDamage, &ftemp, sizeof(float)));
			break;
		case subActive:
			ftemp = 1.0;
			m_effect.push_back(new Effect(e_dotDamage, &ftemp, sizeof(float)));
			break;
		case subPassive:
			break;
		}
		break;
	case shield:
		switch (addType)
		{
		case mainType:
			itemp = 10;
			m_effect.push_back(new Effect(e_shield, &itemp, sizeof(int)));
			break;						 
		case subActive:					 
			itemp = 5;					 
			m_effect.push_back(new Effect(e_shield, &itemp, sizeof(int)));
			break;
		case subPassive:
			itemp = 3;
			m_effect.push_back(new Effect(e_bodyShield, &itemp, sizeof(int)));
			break;
		}
		break;
	}
	return true;
}
