#include "stdafx.h"
#include "SkillObject.h"


void SkillObject::Update(float fTimeElapsed)
{
	m_remainTime -= fTimeElapsed;
	if (isDead()) m_active = false;
}

SkillObject::SkillObject()
{
}


SkillObject::~SkillObject()
{
}
