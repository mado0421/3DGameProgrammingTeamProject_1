#pragma once
class CGameObject
{
	Point3D m_pos;
	Point3D m_direction;
	float m_speed;
	Point3D	m_sector;
public:
	CGameObject();
	~CGameObject();
};

