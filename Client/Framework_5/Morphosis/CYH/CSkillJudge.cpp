#include "stdafx.h"
#include "CSkillJudge.h"


CSkillJudge::CSkillJudge(int skillID, XMFLOAT4X4& world, XMFLOAT3& vector, float radius, float continuedTime, BYTE skillJudgeType,BYTE skillExplosionType):CBullet(0, world, vector, radius, continuedTime, 0)
{
	m_FiredPosition = XMFLOAT3(world._41, world._42, world._43);
	m_FiredDirection = vector;
	m_FiredTime = chrono::system_clock::now();
	m_type = skillJudgeType;
	m_onCollisionType = skillExplosionType;
}


CSkillJudge::~CSkillJudge()
{
}

void CSkillJudge::update(float time)
{
	switch (m_type)
	{
	case linear:
		CBullet::update(time);
		break;
	}
}