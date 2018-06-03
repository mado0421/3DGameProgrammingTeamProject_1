#pragma once

enum effectType:BYTE
{
	e_damage, e_shield, e_buff, e_dotDamage, e_increaseRange, e_bodyShield
};

enum SkillList :BYTE
{
	grenade, poison, shield
};

enum addEffectType :BYTE
{
	mainType, subActive, subPassive
};

struct Effect
{
	BYTE type;
	char info[SKILL_BUFF_SIZE];
	Effect(BYTE b, void* p, int size)
	{
		type = b;
		memcpy(info, p, size);
	}
};

class CSkillEffect
{
public:
	list<Effect*> m_effect;
	CSkillEffect();
	~CSkillEffect();
	bool addEffect(BYTE addType, BYTE kind);
	//이거 타이밍은..? 스킬에서 추가해야겠군
	//실제 스킬 정보도 스킬에서..? 좀 이상한데
	//타이밍만 스킬에서.
	//이거 함수를 메인 만들기 서브 만들기 나눴으면 인자 하나 덜 써도 됨
};

