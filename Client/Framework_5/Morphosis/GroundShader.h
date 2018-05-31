#pragma once
#include "Shader.h"

class GCharacterShader : public IlluminatedTexturedShader
{
private:
	Character			**m_ppCharacter;
	short				m_nCharacter;

	SkillProjectile		**m_ppProjectiles;
	short				m_nProjectiles;

	SkillEffectObject	**m_ppEffectObjects;
	short				m_nEffectObjects;

	Object				**m_ppBullets;
	short				m_nBullets;

public:
	GCharacterShader();
	~GCharacterShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	Character* GetTargetPlayer(int idx) {
		assert(0 <= idx && idx < m_nCharacter);
		return m_ppCharacter[idx];
	}
	virtual void Test() {
		m_ppCharacter[0]->PrintPos();
	}

};
	
class GCollideObjectShader : public IlluminatedTexturedShader
{
private:

public:
	GCollideObjectShader();
	~GCollideObjectShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	//virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

class GDecorateObjectShader : public IlluminatedTexturedShader
{
private:

public:
	GDecorateObjectShader();
	~GDecorateObjectShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	//virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

class GlevelShader : public IlluminatedTexturedShader
{
private:

public:
	GlevelShader();
	~GlevelShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	//virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

class GUIShader : public TexturedShader
{
private:
	Object * m_playerInfoTarget;

public:
	GUIShader();
	~GUIShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

class GSkyboxShader : public TexturedShader
{
private:

public:
	GSkyboxShader();
	~GSkyboxShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

class GParticleShader : public TexturedShader
{
private:

public:
	GParticleShader();
	~GParticleShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};
