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
		m_remainTime = BulletDefaultLifeTime;
		m_active = true;
	}
	virtual void Update(float fTimeElapsed);

public:
	virtual bool isDead() {
		return (m_remainTime <= 0);
	}
};

