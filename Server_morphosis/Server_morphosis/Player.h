#pragma once
#include "stdafx.h"
#include "Object.h"
class CPlayer:public CGameObject
{
	/*Gameobject
	Point3D m_pos;
	Point3D m_direction;
	float m_speed;
	Point3D	m_sector;
	*/
private:
	skillInfo m_skill[MAX_SKILL_NUMBER];
	int		m_weaponInfo;
	int		m_status;
	std::list<CGameObject> m_objects;
	float	m_hp;
public:
	CPlayer();
	~CPlayer();
};

