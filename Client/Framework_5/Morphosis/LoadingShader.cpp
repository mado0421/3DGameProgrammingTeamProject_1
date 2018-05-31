#include "stdafx.h"
#include "LoadingShader.h"
#include "AdvencedMesh.h"

LUIShader::LUIShader()
{
}

LUIShader::~LUIShader()
{
}

void LUIShader::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, void * pContext)
{
	m_nObjects = 1;

	Texture *pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Textures/LT_testImage_00.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, m_nObjects, 1);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, RP_TEXTURE, false);

	m_pMaterial = new Material();
	m_pMaterial->SetTexture(pTexture);
	m_pMaterial->SetReflection(1);

	UIMesh *pUIMesh = new UIMesh(pd3dDevice, pd3dCommandList, -0.0f,-1.0f, 1.0f, 1.0f, 0.0f);

	m_ppObjects = new Object*[m_nObjects];

	Object *UIObject = NULL;
	UIObject = new Object();
	UIObject->SetMesh(0, pUIMesh);
	UIObject->SetPosition(0, 0, 0);
//	UIObject->SetMaterial(m_pMaterial);
	UIObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr/* + (::gnCbvSrvDescriptorIncrementSize)*/);
	m_ppObjects[0] = UIObject;

}

void LUIShader::Update(float fTimeElapsed)
{
}

void LUIShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	TexturedShader::Render(pd3dCommandList, pCamera);

	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j]) m_ppObjects[j]->Render(pd3dCommandList, pCamera);
	}
}
