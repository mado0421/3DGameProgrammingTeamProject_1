#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::Update(float fTimeElapsed)
{
//	MoveForward(BulletDefaultSpeed * fTimeElapsed);
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position = Vector3::Add(xmf3Position, m_movingVector, BulletDefaultSpeed * fTimeElapsed);
	SetPosition(xmf3Position);

	XMFLOAT3 center = XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	m_collisionBox.Center = center;
	XMStoreFloat4(&m_collisionBox.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_collisionBox.Orientation)));

	m_remainTime -= fTimeElapsed;
	if (isDead()) m_active = false;
}
