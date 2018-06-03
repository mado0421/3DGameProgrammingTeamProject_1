#pragma once
#include "Object.h"
#include "Character.h"

#define BulletDefaultLifeTime 1.0f
#define BulletDefaultSpeed	500.0f
class Bullet : public Object
{
public:
	bool m_active = false;
	bool m_team;

	float m_remainTime = BulletDefaultLifeTime;

public:
	Bullet();
	~Bullet();

public:
	virtual void Initialize(Character *owner) {
		SetPosition(owner->GetPosition());
		SetLook(owner->GetLook());
		m_team = owner->m_team;

		//=====================for Test=================
		XMFLOAT3 center = XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
		XMFLOAT3 extents = XMFLOAT3(10.0f, 10.0f, 10.0f);			//반지름 아니고 지름임
		XMFLOAT4 orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);	//w가 1.0f 아니면 터짐
		SetOOBB(center, extents, orientation);

		m_remainTime = BulletDefaultLifeTime;
		m_active = true;
	}
	virtual void Update(float fTimeElapsed);

public:
	virtual bool isDead() {
		return (m_remainTime <= 0);
	}
};

