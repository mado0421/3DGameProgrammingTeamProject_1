#include "stdafx.h"
#include "CGameObject.h"


CGameObject::CGameObject(int objectID)
{
	m_objectID = objectID;
}

CGameObject::CGameObject(int objectID, XMFLOAT4X4 & world, XMFLOAT3 & vector, float radius)
{
	m_objectID = objectID;
	m_world = world;
	m_vector = vector;
	//m_radius = radius;
	m_BS.Center = XMFLOAT3(m_world._41, m_world._42, m_world._43);
	m_BS.Radius = radius;//한번 설정하고 바꿀 일 없잖아?
}

CGameObject::~CGameObject()
{
}

void CGameObject::update(float time)
{
	XMVECTOR temp = XMLoadFloat3(&m_vector);
	temp = XMVector3Normalize(temp);
	XMStoreFloat3(&m_vector, temp);
	m_world._41 += m_vector.x*time;
	m_world._42 += m_vector.y*time;
	m_world._43 += m_vector.z*time;
	m_BS.Center = XMFLOAT3(m_world._41, m_world._42, m_world._43);
}

bool CGameObject::collisionCheck(BoundingSphere & other)
{
	return m_BS.Intersects(other);
}

void CGameObject::initiate( XMFLOAT4X4 & world, XMFLOAT3 & vector, float radius)
{
	m_world = world;
	m_vector = vector;
	//m_radius = radius;
	m_BS.Center = XMFLOAT3(m_world._41, m_world._42, m_world._43);
	m_BS.Radius = radius;//한번 설정하고 바꿀 일 없잖아?
}


