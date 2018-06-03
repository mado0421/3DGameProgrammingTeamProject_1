#include "stdafx.h"
#include "Object.h"


CGameObject::CGameObject()
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::update(float fElapsedTime)
{
	m_pos.x += m_direction.x*m_speed;
	m_pos.y += m_direction.y*m_speed;
	m_pos.z += m_direction.z*m_speed;
}
