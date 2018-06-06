#pragma once
#include "Object.h"
#include "Skill.h"
#include "CYH/CSkill.h"

#define DEFAULTHP 200
#define MAXSLOTLINE 4



class Character : public Object
{
private:
	short			m_curHP;
	short			m_maxHP;
	short			m_speed;
	
	SkillSlotLine	m_SSL[MAXSLOTLINE];

	float           m_fPitch = 0.0f;
	float           m_fYaw = 0.0f;
	float           m_fRoll = 0.0f;

	//=========================for Client Test================================
	float	m_weaponCooltime = 0.5;
	float	m_weaponCurCooltime = 0.0f;



	//=========================for Client Test================================
public:
	bool			m_active	= false;
	bool			m_team		= false;
	int				m_myID;
	bool			m_isconnected = false;
	BYTE			weapon;
	SKILLNUMBER		m_number[3];
	SOCKET			m_sock;
	DWORD			m_direction;
	float			m_cxDelta;
	float			m_cyDelta;

	XMFLOAT3		m_serverPosition;
	float			m_interpolationTime=10.0f;
	//최종 결과에 더해서 포지션 차이를 천천히 줄이는 역할을 한다. 현재위치-서버위치/100해서 더하던가 하자
public:
	Character();
	~Character();

public:
//	virtual void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
//	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
//	void PrintPos() { printf("curPos is %f, %f, %f\n", m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43); }
	virtual void Update(float fTimeElapsed);
	virtual void Rotate(float x, float y, float z);

public:
	void interpolate(float time)
	{
		if (m_interpolationTime > 0)
		{
			XMFLOAT3 temp = XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
			temp = Vector3::Subtract(m_serverPosition, temp);
			temp = Vector3::ScalarProduct(temp, time/10.0f, false);
			Move(temp, true);
			m_interpolationTime -= time;
		}
	}
	void Initialize() {
		//체력 초기화
		m_curHP = m_maxHP = DEFAULTHP;
		m_active = true;
		//=====================for Test=================
		XMFLOAT3 center = XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
		XMFLOAT3 extents = XMFLOAT3(10.0f, 10.0f, 10.0f);			//반지름 아니고 지름임
		XMFLOAT4 orientation = XMFLOAT4(0.0f, 0.0f, 0.0f ,1.0f);	//w가 1.0f 아니면 터짐
		SetOOBB(center, extents, orientation);

		//쿨타임 초기화
		for (int i = 0; i < MAXSLOTLINE; ++i)
			m_SSL[i].ResetCooltime();
	}

	void Damaged(int val) {
		m_curHP -= val;
		if (isDead()) Dead();
	}

	void Dead() {
		//죽었을 때 해줘야 하는 처리들 해주세요!
		m_active = false;
		//3초 기다리기
		//체력이랑 쿨 타임 초기화하기
//		Initialize();
		//본진에서	 리스폰하기
	}

	bool UseWeapon() {
		//애니메이션 재생
		if (!isFireable()) return false;
		m_weaponCurCooltime = m_weaponCooltime;
		return true;
	}
	bool UseSkill(int idx) {
		//스킬 사용
		if (!m_SSL[idx].isUsable())	return false;
		m_SSL[idx].Affect();		return true;

		//애니메이션 재생
	}
	//void Move() {
	//	//애니메이션 재생
	//	//이동방향 바꾸기
	//}

public:
	bool isDead() {
		return (0 >= m_curHP);
	}
	bool isFireable() {
		return (0 >= m_weaponCurCooltime);
	}
};