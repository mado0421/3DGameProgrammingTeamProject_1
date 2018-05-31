#include "stdafx.h"
#include "GroundShader.h"
#include "AdvencedMesh.h"

GCharacterShader::GCharacterShader()
{
}

GCharacterShader::~GCharacterShader()
{
}

void GCharacterShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
	m_nCharacter = MAX_PLAYERS;
	m_ppCharacter = new Character*[m_nCharacter];

	//box_diff
	Texture *pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Textures/box_diff.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nCharacter, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nCharacter, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, RP_TEXTURE, false);

	m_pMaterial = new Material();
	m_pMaterial->SetTexture(pTexture);
	m_pMaterial->SetReflection(1);

	TestMesh *pCharacterMesh = new TestMesh(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < m_nCharacter; ++i) {
		Character *pChar = new Character();
		pChar->SetMesh(0, pCharacterMesh);
		pChar->SetPosition(/*(rand() % 20) - 10.0f, (rand() % 20) - 10.0f, 0.0f*/80.0f * i, 0.0f * i, 10.0f  * i);	// 이 위치가 안 들어가는거 같음
																											// 왜 안 들어가지???
		pChar->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize)*i);
		m_ppCharacter[i] = pChar;
	}

}

void GCharacterShader::Update(float fTimeElapsed)
{
	for (int i = 0; i < m_nCharacter; ++i)
		m_ppCharacter[i]->Update(fTimeElapsed);
}

void GCharacterShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	IlluminatedTexturedShader::Render(pd3dCommandList, pCamera);

	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);

	for (int j = 0; j < m_nCharacter; j++)
	{
		if (m_ppCharacter[j]) m_ppCharacter[j]->Render(pd3dCommandList, pCamera);
	}
}

void GCharacterShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * m_nCharacter, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);

	UINT ncbMaterialBytes = ((sizeof(MATERIALS) + 255) & ~255); //256의 배수
	m_pd3dcbMaterials = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);

}

void GCharacterShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	for (int j = 0; j < m_nCharacter; j++)
	{
		CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (j * ncbElementBytes));
		XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppCharacter[j]->m_xmf4x4World)));
		if (m_pMaterial) pbMappedcbGameObject->m_nMaterial = m_pMaterial->m_nReflection;
	}
}

GlevelShader::GlevelShader()
{
}

GlevelShader::~GlevelShader()
{
}

void GlevelShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
}


void GlevelShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
}

GUIShader::GUIShader()
{
}

GUIShader::~GUIShader()
{
}

void GUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
}

void GUIShader::Update(float fTimeElapsed)
{
}

void GUIShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
}

GSkyboxShader::GSkyboxShader()
{
}

GSkyboxShader::~GSkyboxShader()
{
}

void GSkyboxShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
}

void GSkyboxShader::Update(float fTimeElapsed)
{
}

void GSkyboxShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
}

GParticleShader::GParticleShader()
{
}

GParticleShader::~GParticleShader()
{
}

void GParticleShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
}

void GParticleShader::Update(float fTimeElapsed)
{
}

void GParticleShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
}

GCollideObjectShader::GCollideObjectShader()
{
}

GCollideObjectShader::~GCollideObjectShader()
{
}

void GCollideObjectShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
}

void GCollideObjectShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
}

GDecorateObjectShader::GDecorateObjectShader()
{
}

GDecorateObjectShader::~GDecorateObjectShader()
{
}

void GDecorateObjectShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
}

void GDecorateObjectShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
}
