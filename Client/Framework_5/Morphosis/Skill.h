#pragma once
#include "stdafx.h"
#include "Object.h"


#define MAXSUBELEMENTS 3

struct Skill {
	char		mainElement[16];
	char		subElement[MAXSUBELEMENTS][16];
	int			curNumSubElements = 0;


	Skill() {
		strcpy(mainElement, "");
		for (int i = 0; i < MAXSUBELEMENTS; ++i)
			strcpy(subElement[i], "");
	}
	Skill(const char *element) {
		strcpy(mainElement, element);
		for (int i = 0; i < MAXSUBELEMENTS; ++i)
			strcpy(subElement[i], "");
	}

	void Initialize() {
		strcpy(mainElement, "");
		for (int i = 0; i < MAXSUBELEMENTS; ++i)
			strcpy(subElement[i], "");
		curNumSubElements = 0;
	}

	void ChangeMainElement(const char *element) {
		strcpy(mainElement, element);
	}

	void AddSubElement(const char *element) {
		if (curNumSubElements >= MAXSUBELEMENTS) return;
		strcpy(subElement[curNumSubElements++], element);
	}

	void Affect() {
		//============================================
		//Test
		//cout << mainElement << ", ";
		//for (int i = 0; i < MAXSUBELEMENTS; ++i) {
		//	if ("" == subElement[i]) break;
		//	cout << subElement[i] << ", ";
		//}
		//cout << endl;
		//============================================

	}
};

struct SkillProjectile : public Object {
	float		speed;

public:
	void Effect() {

	}
	void Move(float flowTime) {

	}

	void Update(float flowTime) {

	}
};

struct SkillEffectObject {
	XMFLOAT3 position;

};

#define MAXSKILLSLOT 4

class SkillSlotLine {// ㅁ-ㅁㅁㅁ <-- 요거 한 줄
private:
	Skill		**m_skills;
	int			m_curNumSkill;

	Skill		m_skillOutput;

	float		m_cooltime			= 0.0f;
	float		m_remainCooltime	= 0.0f;
	bool		m_skillUsable;

public:
	SkillSlotLine() { Initialize(); }

public:
	void Initialize() {
		m_skills = new Skill*[MAXSKILLSLOT];

		for (int i = 0; i < MAXSKILLSLOT; ++i) {
			Skill *tempSkill = new Skill();
			tempSkill->Initialize();
			m_skills[i] = tempSkill;
		}

		m_skillOutput.Initialize();
		m_curNumSkill = 0;
		m_skillUsable = true;
	}
	void Affect() {
		if ("" == m_skillOutput.mainElement) { /*cout << "m_skillOutput 없어" << endl;*/ return; }
		m_skillOutput.Affect();
		m_remainCooltime = m_cooltime;
		m_skillUsable = false;
	}
	void Update(float flowTime) {
		if (m_skillUsable) return;
		m_remainCooltime -= flowTime;
		if (isCooltimeDone())m_skillUsable = true;
	}
	void ResetCooltime() {
		m_remainCooltime = 0.0f;
		m_skillUsable = true;
	}

public:
	/*
	기획상 추가는 스킬을 드래그해서 빈 자리가 있는 슬롯에 넣으면 바로 맨 앞 빈 자리를 채우게 함
	제거는 가장 마지막 스킬을 제거
	만들기는 추가나 제거를 한 뒤에 현재 m_skills 배열을 0부터 마지막까지 쭉 훑으면서 주_속성과 부_속성들을 더해서 최종스킬을 만들어서 넣는 과정
	*/	
	void Add(Skill *target) {//m_skills 배열에 0부터 스킬개수-1까지 넣기
		if (isFulled()) return;
		m_skills[m_curNumSkill++] = target;

		MakeOutput();
	}	
	void Sub() {//m_skills 배열에서 0까지 하나씩 Sub
		if (isBlanked()) return;
		delete m_skills[m_curNumSkill];
		m_skills[m_curNumSkill--] = nullptr;

		MakeOutput();
	}
	void MakeOutput() {// 만들어서 최종스킬에 넣기
		m_skillOutput.Initialize();
		m_skillOutput.ChangeMainElement(m_skills[0]->mainElement);
		for (int i = 0; i < MAXSUBELEMENTS; ++i)
			m_skillOutput.AddSubElement(m_skills[i + 1]->mainElement);
	}

public:
	bool isFulled() {
		//ㅁ-ㅁㅁㅁ 이 전부 꽉 참
		return (m_curNumSkill == MAXSKILLSLOT);
	}
	bool isBlanked() {
		//ㅁ-ㅁㅁㅁ 이 전부 비어있음
		//for (int i = 0; i < 스킬개수; ++i)
		//	if (nullptr == m_skills[i]) return true;
		//return false;
		return (m_curNumSkill == 0);
	}
	bool isCooltimeDone() {
		return m_remainCooltime <= 0.0f;
	}
	bool isUsable() {
		return m_skillUsable;
	}
};