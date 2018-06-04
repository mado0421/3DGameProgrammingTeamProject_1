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
	m_pd3dDevice		= pd3dDevice;
	m_pd3dCommandList	= pd3dCommandList;

	m_nCharacter		= MAX_PLAYERS;
	m_nBullets			= m_nCharacter * BulletPC;
	m_nProjectile		= m_nCharacter * ProjectilePC;
	m_nSkillObject		= m_nCharacter * SkillObjectPC;

	m_ppCharacter		= new Character*[m_nCharacter];
	m_ppBullets			= new Bullet*[m_nBullets];
	m_ppProjectile		= new SkillProjectile*[m_nProjectile];
	m_ppSkillObject		= new SkillObject*[m_nSkillObject];

	//box_diff
	Texture *pTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
	pTexture->LoadTextureFromFile(m_pd3dDevice, m_pd3dCommandList, L"Assets/Textures/box_diff.dds", 0);
	//pTexture->LoadTextureFromFile(m_pd3dDevice, m_pd3dCommandList, L"Assets/Textures/UITest.dds", 0); 

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	CreateCbvAndSrvDescriptorHeaps(m_pd3dDevice, m_pd3dCommandList, m_nCharacter, 1);
	CreateShaderVariables(m_pd3dDevice, m_pd3dCommandList);
	CreateConstantBufferViews(m_pd3dDevice, m_pd3dCommandList, m_nCharacter, m_pd3dcbGameObjects, ncbElementBytes);
	CreateShaderResourceViews(m_pd3dDevice, m_pd3dCommandList, pTexture, RP_TEXTURE, false);

	m_pMaterial = new Material();
	m_pMaterial->SetTexture(pTexture);
	m_pMaterial->SetReflection(1);

	TestMesh *pTestMesh = new TestMesh(pd3dDevice, pd3dCommandList);
	TestMesh *pBulletMesh = new TestMesh(pd3dDevice, pd3dCommandList, 5.0f);
	TestMesh *pProjeectileMesh = new TestMesh(pd3dDevice, pd3dCommandList, 7.5f);
	TestMesh *pSkillObjectMesh = new TestMesh(pd3dDevice, pd3dCommandList, 35.0f);


	// character preMake
	for (int i = 0; i < m_nCharacter; ++i) {
		Character *pChar = new Character();
		pChar->SetMesh(0, pTestMesh);
		pChar->SetPosition(/*(rand() % 2000) - 1000.0f, 0.0f, (rand() % 2000) - 1000.0f*/80.0f * (i), 0.0f * i, 0.0f  * i);
		pChar->Initialize();
		if (i >= m_nCharacter / 2) pChar->m_team = true;
		pChar->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize)*i);
		m_ppCharacter[i] = pChar;
	}

	for (int i = 0; i < m_nCharacter; ++i) {
		for (int j = 0; j < BulletPC; ++j) {
			//bullet's m_active is false
			Bullet *pBullet = new Bullet();
			pBullet->SetMesh(0, pBulletMesh);
			pBullet->SetPosition(m_ppCharacter[i]->GetPosition());
			pBullet->SetCbvGPUDescriptorHandlePtr(
				m_d3dCbvGPUDescriptorStartHandle.ptr + 					//캐릭터 다음 불릿
				(::gnCbvSrvDescriptorIncrementSize) * m_nCharacter + 
				(::gnCbvSrvDescriptorIncrementSize) * (BulletPC * i) +
				(::gnCbvSrvDescriptorIncrementSize) * j);
			m_ppBullets[i * BulletPC + j] = pBullet;
		}

		for (int j = 0; j < ProjectilePC; ++j) {
			SkillProjectile *pProj = new SkillProjectile();
			pProj->SetMesh(0, pProjeectileMesh);
			pProj->SetPosition(m_ppCharacter[i]->GetPosition());
			pProj->SetCbvGPUDescriptorHandlePtr(
				m_d3dCbvGPUDescriptorStartHandle.ptr +					//캐릭터 다음 불릿 다음 스킬 투사체
				(::gnCbvSrvDescriptorIncrementSize) * m_nCharacter +
				(::gnCbvSrvDescriptorIncrementSize) * m_nBullets +
				(::gnCbvSrvDescriptorIncrementSize) * (ProjectilePC * i) +
				(::gnCbvSrvDescriptorIncrementSize) * j);
			m_ppProjectile[i * ProjectilePC + j] = pProj;
		}

		for (int j = 0; j < SkillObjectPC; ++j) {
			SkillObject *pSO = new SkillObject();
			pSO->SetMesh(0, pSkillObjectMesh);
			pSO->SetPosition(m_ppCharacter[i]->GetPosition());
			pSO->SetCbvGPUDescriptorHandlePtr(
				m_d3dCbvGPUDescriptorStartHandle.ptr +					//캐릭터 다음 불릿 다음 스킬 투사체 다음 스킬 오브젝트~
				(::gnCbvSrvDescriptorIncrementSize) * m_nCharacter +
				(::gnCbvSrvDescriptorIncrementSize) * m_nBullets +
				(::gnCbvSrvDescriptorIncrementSize) * m_nProjectile +
				(::gnCbvSrvDescriptorIncrementSize) * (SkillObjectPC * i) +
				(::gnCbvSrvDescriptorIncrementSize) * j);
			m_ppSkillObject[i * SkillObjectPC + j] = pSO;
		}
	}

}

void GCharacterShader::Update(float fTimeElapsed)
{
	// Update Movement
	for (int i = 0; i < m_nCharacter; ++i)
	{
		DWORD dwDirection = m_ppCharacter[i]->m_direction;
		if(dwDirection!=0)
			m_ppCharacter[i]->Move(dwDirection, 100.0f * fTimeElapsed, true);
	}
		
	for (int i = 0; i < m_nCharacter; ++i)		if (m_ppCharacter[i]->m_active)		m_ppCharacter[i]->Update(fTimeElapsed);
	for (int i = 0; i < m_nBullets; ++i)		if (m_ppBullets[i]->m_active)		m_ppBullets[i]->Update(fTimeElapsed);
	for (int i = 0; i < m_nProjectile; ++i)		if (m_ppProjectile[i]->m_active)	m_ppProjectile[i]->Update(fTimeElapsed);
	for (int i = 0; i < m_nSkillObject; ++i)	if (m_ppSkillObject[i]->m_active)	m_ppSkillObject[i]->Update(fTimeElapsed);

	// Collision Check
	// 일단은 캐릭터와 불릿이랑 하자
	for (int i = 0; i < m_nCharacter; ++i)
		if (m_ppCharacter[i]->m_active) {
			for (int j = 0; j < m_nBullets; ++j) 
				if(m_ppBullets[j]->m_active)
					if(m_ppCharacter[i]->m_team != m_ppBullets[j]->m_team)
						if (m_ppCharacter[i]->isCollide(m_ppBullets[j]->m_collisionBox)) {
							m_ppBullets[j]->m_active = false;
							m_ppCharacter[i]->m_active = false;
							break;
						}
			for (int j = 0; j < m_nProjectile; ++j)
				if(m_ppProjectile[j]->m_active)
					if(m_ppCharacter[i]->m_team != m_ppProjectile[j]->m_team)
						if (m_ppCharacter[i]->isCollide(m_ppProjectile[j]->m_collisionBox)) {
							m_ppProjectile[j]->m_active = false;

							for (int k = 0; k < SkillObjectPC; ++k) {
								if (!m_ppSkillObject[j]->m_active) {
									m_ppSkillObject[j]->Initialize(m_ppProjectile[k], m_ppProjectile[j]->m_playerId);
									break;
								}
							}
						}
		}
}

void GCharacterShader::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	IlluminatedTexturedShader::Render(pd3dCommandList, pCamera);

	if (m_pMaterial) m_pMaterial->UpdateShaderVariables(pd3dCommandList);

	for (int j = 0; j < m_nCharacter; j++)		if (m_ppCharacter[j]->m_active)		m_ppCharacter[j]->Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nBullets; ++i)		if (m_ppBullets[i]->m_active)		m_ppBullets[i]->Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nProjectile; ++i)		if (m_ppProjectile[i]->m_active)	m_ppProjectile[i]->Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nSkillObject; ++i)	if (m_ppSkillObject[i]->m_active)	m_ppSkillObject[i]->Render(pd3dCommandList, pCamera);


}

void GCharacterShader::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	int numOfObject = m_nCharacter + (m_nCharacter * BulletPC) + (m_nCharacter * ProjectilePC) + (m_nCharacter * SkillObjectPC);
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes * numOfObject, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbGameObjects->Map(0, NULL, (void **)&m_pcbMappedGameObjects);



}

void GCharacterShader::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
//	int numOfObject = m_nCharacter + (m_nCharacter * BulletPC) + (m_nCharacter * ProjectilePC) + (m_nCharacter * SkillObjectPC);
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);

	// Upload Character Info
	for (int i = 0; i < m_nCharacter; i++)
	{
		if (m_ppCharacter[i]->m_active) {
			CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (i * ncbElementBytes));	
			XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppCharacter[i]->m_xmf4x4World)));
			if (m_pMaterial) pbMappedcbGameObject->m_nMaterial = m_pMaterial->m_nReflection;
		}
	}

	// Upload bullet Info
	for (int i = 0; i < m_nBullets; i++)
	{
		if (m_ppBullets[i]->m_active) {
			CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (m_nCharacter * ncbElementBytes ) + (i * ncbElementBytes));
			XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppBullets[i]->m_xmf4x4World)));
			if (m_pMaterial) pbMappedcbGameObject->m_nMaterial = m_pMaterial->m_nReflection;
		}
	}

	// Upload Projectiles Info
	for (int i = 0; i < m_nProjectile; i++)
	{
		if (m_ppProjectile[i]->m_active) {
			CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (m_nCharacter * ncbElementBytes) + (m_nBullets * ncbElementBytes) + (i * ncbElementBytes));
			XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppProjectile[i]->m_xmf4x4World)));
			if (m_pMaterial) pbMappedcbGameObject->m_nMaterial = m_pMaterial->m_nReflection;
		}
	}

	// Upload SkillObject Info
	for (int i = 0; i < m_nSkillObject; i++)
	{
		if (m_ppSkillObject[i]->m_active) {
			CB_GAMEOBJECT_INFO *pbMappedcbGameObject = (CB_GAMEOBJECT_INFO *)((UINT8 *)m_pcbMappedGameObjects + (m_nCharacter * ncbElementBytes) + (m_nBullets * ncbElementBytes) + (m_nProjectile * ncbElementBytes) + (i * ncbElementBytes));
			XMStoreFloat4x4(&pbMappedcbGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppSkillObject[i]->m_xmf4x4World)));
			if (m_pMaterial) pbMappedcbGameObject->m_nMaterial = m_pMaterial->m_nReflection;
		}
	}
}

void GCharacterShader::CreateCbvAndSrvDescriptorHeaps(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nCharacter, int nShaderResourceViews)
{
	int nConstantBufferViews = nCharacter + (nCharacter * BulletPC) + (nCharacter * ProjectilePC) + (nCharacter * SkillObjectPC);

	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = nConstantBufferViews + nShaderResourceViews; //CBVs + SRVs 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void **)&m_pd3dCbvSrvDescriptorHeap);

	m_d3dCbvCPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dCbvGPUDescriptorStartHandle = m_pd3dCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	m_d3dSrvCPUDescriptorStartHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);
	m_d3dSrvGPUDescriptorStartHandle.ptr = m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * nConstantBufferViews);

}

void GCharacterShader::CreateConstantBufferViews(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, int nCharacter, ID3D12Resource * pd3dConstantBuffers, UINT nStride)
{
	int nConstantBufferViews = nCharacter + (nCharacter * BulletPC) + (nCharacter * ProjectilePC) + (nCharacter * SkillObjectPC);

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = pd3dConstantBuffers->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	d3dCBVDesc.SizeInBytes = nStride;
	for (int j = 0; j < nConstantBufferViews; j++)
	{
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress + (nStride * j);
		D3D12_CPU_DESCRIPTOR_HANDLE d3dCbvCPUDescriptorHandle;
		d3dCbvCPUDescriptorHandle.ptr = m_d3dCbvCPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * j);
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorHandle);
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
