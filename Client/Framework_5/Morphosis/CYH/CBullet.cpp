#include "stdafx.h"
#include "CBullet.h"


CBullet::CBullet(int bulletID, XMFLOAT4X4 & world, XMFLOAT3 & vector, float radius, float continuedTime,float dmg):CGameObject(0,world,vector,radius)
{
	m_bulletID = bulletID;
	m_continueTime = continuedTime;
	m_damage = dmg;
	m_start = std::chrono::system_clock::now();
}

CBullet::~CBullet()
{
}

void CBullet::update(float time)
{
	CGameObject::update(time);
	m_continueTime -= time;
}

bool CBullet::isDead()
{
	std::chrono::duration<float> temp= std::chrono::system_clock::now() - m_start;
	return m_continueTime < temp.count();
}
