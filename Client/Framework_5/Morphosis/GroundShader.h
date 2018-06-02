#pragma once
#include "Shader.h"
#include "Character.h"
#include "Bullet.h"
#include "SkillProjectile.h"
#include "SkillEffectObject.h"


#define BulletPC		32
#define ProjectilePC	8
#define EffectObjectPC	16
class GCharacterShader : public IlluminatedTexturedShader
{
private:
	//ID3D12Resource		*m_pd3dcbCharacters = NULL;
	//CB_GAMEOBJECT_INFO	*m_pcbMappedCharacters = NULL;
	Character				**m_ppCharacter;
	short					m_nCharacter;

	//ID3D12Resource		*m_pd3dcbProjectiles = NULL;
	//CB_GAMEOBJECT_INFO	*m_pcbMappedProjectiles = NULL;
	SkillProjectile			**m_ppProjectiles;
	short					m_nProjectiles;

	//ID3D12Resource		*m_pd3dcbEffectObjects = NULL;
	//CB_GAMEOBJECT_INFO	*m_pcbMappedEffectObjects = NULL;
	SkillEffectObject		**m_ppEffectObjects;
	short					m_nEffectObjects;

	//ID3D12Resource		*m_pd3dcbBullets = NULL;
	//CB_GAMEOBJECT_INFO	*m_pcbMappedBullets = NULL;
	Bullet					**m_ppBullets;
	short					m_nBullets;

	ID3D12Device				*m_pd3dDevice = NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList = NULL;

public:
	GCharacterShader();
	~GCharacterShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
	virtual void CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void CreateCbvAndSrvDescriptorHeaps(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, int nShaderResourceViews);
	virtual void CreateConstantBufferViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int nConstantBufferViews, ID3D12Resource *pd3dConstantBuffers, UINT nStride);
//	virtual void CreateShaderResourceViews(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, Texture *pTexture, UINT nRootParameterStartIndex, bool bAutoIncrement);

public:
	Character* GetTargetPlayer(int idx) {
		assert(0 <= idx && idx < m_nCharacter);
		return m_ppCharacter[idx];
	}
	void AddBullet(int idx) {
		assert(0 <= idx && idx < m_nCharacter);
		////일단 나가는지 확인해보자.
		//m_ppBullets[0]->Initialize(m_ppCharacter[0]);	// 나가는거 확인!!

		/*
		캐릭터 별 현재 발사한 총알 개수 받아서 (nCurVisibleBullet)
		그걸 넣어줘야 함
		*/
		for (int i = 0; i < BulletPC; ++i) {
			if (!m_ppBullets[(m_nCharacter * idx) + i]->m_active) {
				m_ppBullets[(m_nCharacter * idx) + i]->Initialize(m_ppCharacter[idx]);
				break;
			}
		}
	}
	//virtual void Test() {
	//	m_ppCharacter[0]->PrintPos();
	//}
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
