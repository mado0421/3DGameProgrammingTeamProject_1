#pragma once
#include "CGameObject.h"
class CBullet:public CGameObject
{
public:
	int m_bulletID;
	std::chrono::system_clock::time_point m_start;
	float m_continueTime;
	float m_damage;
	CBullet() = delete;
	CBullet(int objectID, XMFLOAT4X4& world, XMFLOAT3& vector, float radius, float continuedTime,float dmg);
	~CBullet();
	virtual void update(float time);
	bool isDead();
};

