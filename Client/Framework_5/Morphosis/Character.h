#pragma once
#include "Object.h"
#include "Skill.h"

#define DEFAULTHP 200
#define MAXSLOTLINE 4

class Character : public Object
{
private:
	XMFLOAT3		m_xmf3Position	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3		m_xmf3Right		= XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3		m_xmf3Up		= XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3		m_xmf3Look		= XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3		m_xmf3Velocity	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     	m_xmf3Gravity	= XMFLOAT3(0.0f, 0.0f, 0.0f);

	float           m_fMaxVelocityXZ	= 400.0f;
	float           m_fMaxVelocityY		= 400.0f;
	float           m_fFriction			= 200.0f;

	short			m_curHP;
	short			m_maxHP;
	short			m_speed;
	bool			m_team;
	int				m_myID;
	SkillSlotLine	m_SSL[MAXSLOTLINE];

public:
	Character();
	~Character();

public:
	void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void PrintPos() { printf("curPos is %f, %f, %f\n", m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43); }
	virtual void Update(float fTimeElapsed);

public:
	void Initialize() {
		//체력 초기화
		m_curHP = m_maxHP = DEFAULTHP;

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
		//3초 기다리기
		//체력이랑 쿨 타임 초기화하기
		Initialize();
		//본진에서 리스폰하기
	}

	bool UseWeapon() {
		//애니메이션 재생
	}
	bool UseSkill(int idx) {
		//스킬 사용
		if (!m_SSL[idx].isUsable())	return false;
		m_SSL[idx].Affect();		return true;

		//애니메이션 재생
	}
	void Move() {
		//애니메이션 재생
		//이동방향 바꾸기
	}

public:
	bool isDead() {
		return (m_curHP >= 0);
	}
};