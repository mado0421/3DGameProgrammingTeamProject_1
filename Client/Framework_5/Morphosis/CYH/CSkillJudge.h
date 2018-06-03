#pragma once
#include "CBullet.h"
enum missileType: BYTE
{
	linear, target, construct
};

enum explosionType : BYTE
{
	none, range
};

class CSkillJudge :
	public CBullet
{
	//이것도 충돌 판정은 room에서 한다.
public:
	BYTE m_type;
	BYTE m_onCollisionType;
	XMFLOAT3 m_FiredPosition;
	XMFLOAT3 m_FiredDirection;
	std::chrono::system_clock::time_point m_FiredTime;
	CSkillJudge() = delete;
	CSkillJudge(int skillID, XMFLOAT4X4& world, XMFLOAT3& vector, float radius, float continuedTime, BYTE skillJudgeType, BYTE skillExplosionType);
	~CSkillJudge();
	virtual void update(float time);
	
};

