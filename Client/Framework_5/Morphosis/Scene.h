#pragma once
#include "Shader.h"
#include "LoadingShader.h"
#include "GroundShader.h"
#include "Camera.h"

enum Scenes {
	Title=0,
	EnterRoom,
	Matching,
	Play,
	Result,

	NumOfScenes
};

enum GShaders {
	_Character=0,
	_CollObj,
	_DecoObj,
	_Level,
	_UI,
	_Skybox,
	_Particle,

	NumOfGShader
};

struct LIGHT
{
	XMFLOAT4				m_xmf4Ambient;
	XMFLOAT4				m_xmf4Diffuse;
	XMFLOAT4				m_xmf4Specular;
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff;
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation;
	float					m_fPhi; //cos(m_fPhi)
	bool					m_bEnable;
	int						m_nType;
	float					m_fRange;
	float					padding;
};

struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_xmf4GlobalAmbient;
};

class Scene
{
protected:
	ID3D12RootSignature		* m_pd3dGraphicsRootSignature	= NULL;

	Shader					**m_ppShaders;
	int						m_nShaders;

	CCamera					*m_pCamera						= NULL;
public:
	Scene();
	~Scene();

public:
	virtual void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Update(float fTimeElapsed);
	virtual bool ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed);

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
};

class LoadingScene : public Scene
{
protected:

public:
	LoadingScene();
	~LoadingScene();

public:
	virtual void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	//virtual void Update(float fTimeElapsed);
	//virtual bool ProcessInput(UCHAR *pKeysBuffer);
};

class GroundScene : public Scene
{
protected:
	Character				*m_pPlayer = NULL;

	LIGHTS					*m_pLightsArr = NULL;

	ID3D12Resource			*m_pd3dcbLights = NULL;
	LIGHTS					*m_pcbMappedLights = NULL;

	MATERIALS				*m_pMaterials = NULL;
	int						m_nMaterials = 0;
	ID3D12Resource			*m_pd3dcbMaterials = NULL;
	MATERIAL				*m_pcbMappedMaterials = NULL;

	//======for using===========
	GCharacterShader *pGCS = NULL;

public:
	GroundScene();
	~GroundScene();

public:
	void BuildLights();
	void BuildMaterials();

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandListe);
	virtual void Update(float fTimeElapsed);
	virtual bool ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed);
};

class TitleScene : public LoadingScene
{
private:

public:
	TitleScene();
	~TitleScene();
public:
	virtual void Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);

};

class EnterRoomScene : public GroundScene
{
private:

public:
	EnterRoomScene();
	~EnterRoomScene();
public:

};

class MatchingScene : public LoadingScene
{
private:

public:
	MatchingScene();
	~MatchingScene();
public:

};

class PlayScene : public GroundScene
{
private:

public:
	PlayScene();
	~PlayScene();
public:

};

class ResultScene : public LoadingScene
{
private:

public:
	ResultScene();
	~ResultScene();
public:

};