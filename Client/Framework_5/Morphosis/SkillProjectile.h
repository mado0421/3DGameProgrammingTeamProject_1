#pragma once
#include "Object.h"

class SkillProjectile : public Object
{
public:
	bool m_active = false;
	bool m_team;

public:
	SkillProjectile();
	~SkillProjectile();
};

