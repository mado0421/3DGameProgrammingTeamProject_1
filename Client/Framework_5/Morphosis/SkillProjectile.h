#pragma once
#include "Bullet.h"

class SkillProjectile : public Bullet
{
public:
	char m_playerId;

public:
	virtual void Initialize(Character *owner, int ID) {
		m_playerId = ID;
		SetPosition(owner->GetPosition());
		m_movingVector = owner->GetLook();
		m_movingVector = Vector3::Normalize(m_movingVector);

		m_team = owner->m_team;

		//=====================for Test=================
		XMFLOAT3 center = XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
		XMFLOAT3 extents = XMFLOAT3(15.0f, 15.0f, 15.0f);			//반지름 아니고 지름임
		XMFLOAT4 orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);	//w가 1.0f 아니면 터짐
		SetOOBB(center, extents, orientation);

		m_remainTime = BulletDefaultLifeTime;
		m_active = true;
	}

public:
	SkillProjectile();
	~SkillProjectile();
};

