#pragma once
#include "Object.h"

class SkillEffectObject : public Object
{
public:
	bool m_active = false;
	bool m_team;

public:
	SkillEffectObject();
	~SkillEffectObject();
};

