//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"
#include"Player.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMaterial::CMaterial()
{
	m_xmf4Albedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

CMaterial::~CMaterial()
{
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}

void CMaterial::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTexture::CTexture(int nTextures, UINT nTextureType, int nSamplers)
{
	m_nTextureType = nTextureType;
	m_nTextures = nTextures;
	if (m_nTextures > 0)
	{
		m_pRootArgumentInfos = new SRVROOTARGUMENTINFO[m_nTextures];
		m_ppd3dTextureUploadBuffers = new ID3D12Resource*[m_nTextures];
		m_ppd3dTextures = new ID3D12Resource*[m_nTextures];
	}

	m_nSamplers = nSamplers;
	if (m_nSamplers > 0) m_pd3dSamplerGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nSamplers];
}

CTexture::~CTexture()
{
	if (m_ppd3dTextures)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextures[i]) m_ppd3dTextures[i]->Release();
	}

	if (m_pRootArgumentInfos)
	{
		delete[] m_pRootArgumentInfos;
	}

	if (m_pd3dSamplerGpuDescriptorHandles) delete[] m_pd3dSamplerGpuDescriptorHandles;
}

void CTexture::SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pRootArgumentInfos[nIndex].m_nRootParameterIndex = nRootParameterIndex;
	m_pRootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle = d3dSrvGpuDescriptorHandle;
}

void CTexture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	m_pd3dSamplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void CTexture::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	for (int i = 0; i < m_nTextures; i++)
	{
		pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[i].m_nRootParameterIndex, m_pRootArgumentInfos[i].m_d3dSrvGpuDescriptorHandle);
	}
}

void CTexture::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, int nIndex)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(m_pRootArgumentInfos[nIndex].m_nRootParameterIndex, m_pRootArgumentInfos[nIndex].m_d3dSrvGpuDescriptorHandle);
}

void CTexture::ReleaseUploadBuffers()
{
	if (m_ppd3dTextureUploadBuffers)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextureUploadBuffers[i]) m_ppd3dTextureUploadBuffers[i]->Release();
		delete[] m_ppd3dTextureUploadBuffers;
		m_ppd3dTextureUploadBuffers = NULL;
	}
}

void CTexture::ReleaseShaderVariables()
{
}

void CTexture::LoadTextureFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, UINT nIndex)
{
	m_ppd3dTextures[nIndex] = ::CreateTextureResourceFromFile(pd3dDevice, pd3dCommandList, pszFileName, &m_ppd3dTextureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject()
{
	m_xmf4x4World = Matrix4x4::Identity();

	
}

CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pMaterial) m_pMaterial->Release();
	if (m_pTexture)m_pTexture->Release();
}

void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CGameObject::SetMesh(int nIndex, CMesh * pMesh)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}
}

void CGameObject::SetShader(CShader *pShader)
{
	if (!m_pMaterial)
	{
		if (!m_pShader)
		{
			m_pShader = pShader;
			m_pShader->AddRef();
		}
		else
		{
			m_pShader->Release();
			m_pShader = pShader;
			m_pShader->AddRef();
		}
	}

	if (m_pMaterial) m_pMaterial->SetShader(pShader);
}

void CGameObject::SetTexture(CTexture * pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::SetMaterial(UINT nReflection)
{
	if (!m_pMaterial) m_pMaterial = new CMaterial();
	m_pMaterial->m_nReflection = nReflection;
}

void CGameObject::Animate(float fTimeElapsed)
{
	OnPrepareRender();
	m_xmOOBBTransformed.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
	XMStoreFloat4(&m_xmOOBBTransformed.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBBTransformed.Orientation)));
}

void CGameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender();

	if (m_pTexture)m_pTexture->UpdateShaderVariables(pd3dCommandList);

	if (m_pMaterial)
	{
		if (m_pMaterial->m_pShader)
		{
			m_pMaterial->m_pShader->Render(pd3dCommandList, pCamera);
			m_pMaterial->m_pShader->UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);
		}
	}

	if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);

	if (m_pMesh) m_pMesh->Render(pd3dCommandList);


	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList);
		}
	}
}

void CGameObject::ReleaseUploadBuffers()
{
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
	if (m_pTexture)m_pTexture->ReleaseUploadBuffers();
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CGameObject::SetLook(float x, float y, float z)
{
	m_xmf4x4World._31 = x;
	m_xmf4x4World._32 = y;
	m_xmf4x4World._33 = z;
}

void CGameObject::SetLook(XMFLOAT3 xmf3Look)
{
	SetLook(xmf3Look.x, xmf3Look.y, xmf3Look.z);
}

void CGameObject::SetUp(float x, float y, float z)
{
	m_xmf4x4World._21 = x;
	m_xmf4x4World._22 = y;
	m_xmf4x4World._23 = z;
}

void CGameObject::SetUp(XMFLOAT3 xmf3Up)
{
	SetUp(xmf3Up.x, xmf3Up.y, xmf3Up.z);
}

void CGameObject::SetRight(float x, float y, float z)
{
	m_xmf4x4World._11 = x;
	m_xmf4x4World._12 = y;
	m_xmf4x4World._13 = z;
}

void CGameObject::SetRight(XMFLOAT3 xmf3Light)
{
	SetRight(xmf3Light.x, xmf3Light.y, xmf3Light.z);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)));
}

XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainObject::CTerrainObject()
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fRotationSpeed = 0.f;
}

CTerrainObject::~CTerrainObject()
{
}

void CTerrainObject::OnObjectUpdateCallback(float fTimeElapsed)
{
	XMFLOAT3 xmf3ObjectPosition = GetPosition();
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pObjectUpdatedContext;
	
	float fHeight = pTerrain->GetHeight(xmf3ObjectPosition.x, xmf3ObjectPosition.z) +
		m_UpOffset;
	
	xmf3ObjectPosition.y = fHeight;
	SetPosition(xmf3ObjectPosition);
	//if (xmf3ObjectPosition.y < fHeight){}
}

void CTerrainObject::Animate(float fTimeElapsed)
{
	if (m_Move) {
		if (m_fRotationSpeed)CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed*fTimeElapsed);
		XMFLOAT3 xmf3Position = GetPosition();
		xmf3Position = Vector3::Add(xmf3Position, m_xmf3MovingDirection, fTimeElapsed*60);
		CGameObject::SetPosition(xmf3Position);

		//MoveForward(fTimeElapsed * 30);
	}
	OnObjectUpdateCallback(fTimeElapsed);

	CGameObject::Animate(fTimeElapsed);
}

void CTerrainObject::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRotatingObject::CRotatingObject()
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
}

void CRotatingObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CRevolvingObject::CRevolvingObject()
{
	m_xmf3RevolutionAxis = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_fRevolutionSpeed = 0.0f;
}

CRevolvingObject::~CRevolvingObject()
{
}

void CRevolvingObject::Animate(float fTimeElapsed)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3RevolutionAxis), XMConvertToRadians(m_fRevolutionSpeed * fTimeElapsed));
	m_xmf4x4World = Matrix4x4::Multiply(m_xmf4x4World, mtxRotate);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//

CHeightMapTerrain::CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList
	*pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LPCTSTR pFileName, int
	nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4
	xmf4Color) : CGameObject()
{
	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
	m_nWidth = nWidth;
	m_nLength = nLength;

	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크
	기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	//xmf3Scale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다.
	m_xmf3Scale = xmf3Scale;

	//지형에 사용할 높이 맵을 생성한다.
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다.
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
	m_nMeshes = cxBlocks * czBlocks;

	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.
	m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;

	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{

			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치(좌표)이다.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);

			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, pd3dCommandList, xStart,
				zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage);
			SetMesh(x + (z*cxBlocks), pHeightMapGridMesh);
		}
	}
	
	//지형을 렌더링하기 위한 셰이더를 생성한다.
	//CTerrainShader *pShader = new CTerrainShader();
	//pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

	CTexture *pTerrainTexture = new CTexture(2, RESOURCE_TEXTURE2D, 0);

	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Terrain/Base_Texture.dds", 0);
	pTerrainTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Terrain/Detail_Texture_7.dds", 1);
	//UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수

	CTextureTerrainShader *pTerrainShader = new CTextureTerrainShader();
	pTerrainShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	pTerrainShader->CreateShaderVariables(pd3dDevice,pd3dCommandList);
	pTerrainShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 2);
	pTerrainShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTerrainTexture, 7, true);

	m_pTexture = pTerrainTexture;
	pTerrainShader->AddRef();

	m_pShader = pTerrainShader;
	pTerrainShader->AddRef();

	// 텍스쳐 등록
}


CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage) delete m_pHeightMapImage;
}

void CHeightMapTerrain::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{

	//D3D12_GPU_VIRTUAL_ADDRESS d3dcbGameObjectGpuVirtualAddress = m_pd3dcbGameObject->GetGPUVirtualAddress();

	OnPrepareRender();

	if (m_pShader)
	{
		m_pShader->Render(pd3dCommandList, pCamera);
		m_pShader->UpdateShaderVariables(pd3dCommandList);
	}
	//pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbGameObjectGpuVirtualAddress);
	if (m_pTexture) {
		
		m_pTexture->UpdateShaderVariables(pd3dCommandList);
	}

	if (m_pMesh) m_pMesh->Render(pd3dCommandList);

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList);
		}
	}
}

CBillBoardObject::CBillBoardObject()
{
}

CBillBoardObject::~CBillBoardObject()
{
}

void CBillBoardObject::Animate(float fTimeElapsed)
{
	if (m_Move) {
		if (m_fRotationSpeed)CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed*fTimeElapsed);
		XMFLOAT3 xmf3Position = GetPosition();
		xmf3Position = Vector3::Add(xmf3Position, m_xmf3MovingDirection, fTimeElapsed * 60);
		CGameObject::SetPosition(xmf3Position);

		//MoveForward(fTimeElapsed * 30);
	}
	OnObjectUpdateCallback(fTimeElapsed);

	CGameObject::Animate(fTimeElapsed);
}
//class CTerrainPlayer;
void CBillBoardObject::OnObjectUpdateCallback(float fTimeElapsed)
{
	XMFLOAT3 xmf3ObjectPosition = GetPosition();
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pObjectUpdatedContext;
	CCamera *pCamera = (CCamera*)m_pCameraUpdateContext;

	float fHeight = pTerrain->GetHeight(xmf3ObjectPosition.x, xmf3ObjectPosition.z) +
		m_UpOffset;

	if (pCamera) {
		SetLook(pCamera->GetLookVector());
		SetRight(pCamera->GetRightVector());
	}
	xmf3ObjectPosition.y = fHeight;
	SetPosition(xmf3ObjectPosition);
	//if (xmf3ObjectPosition.y < fHeight){}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSkyBox::CSkyBox(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
{
	m_nMeshes = 6;
	m_ppMeshes = new CMesh*[6];

	for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;

	CTexturedRectMesh *pSkyBoxMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, +10.0f);
	SetMesh(0, pSkyBoxMesh);
	pSkyBoxMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, -10.0f);
	SetMesh(1, pSkyBoxMesh);
	pSkyBoxMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 0.0f, 20.0f, 20.0f, -10.0f, 0.0f, 0.0f);
	SetMesh(2, pSkyBoxMesh);
	pSkyBoxMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 0.0f, 20.0f, 20.0f, +10.0f, 0.0f, 0.0f);
	SetMesh(3, pSkyBoxMesh);
	pSkyBoxMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, 0.0f, 20.0f, 0.0f, +10.0f, 0.0f);
	SetMesh(4, pSkyBoxMesh);
	pSkyBoxMesh = new CTexturedRectMesh(pd3dDevice, pd3dCommandList, 20.0f, 0.0f, 20.0f, 0.0f, -10.0f, 0.0f);
	SetMesh(5, pSkyBoxMesh);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbGameObject = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbGameObject->Map(0, NULL, (void **)&m_pcbMappedGameObject);

	CTexture *pSkyBoxTexture = new CTexture(6, RESOURCE_TEXTURE2D, 0);

	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/SkyBox/SkyBox_Front_0.dds", 0);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/SkyBox/SkyBox_Back_0.dds", 1);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/SkyBox/SkyBox_Left_0.dds", 2);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/SkyBox/SkyBox_Right_0.dds", 3);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/SkyBox/SkyBox_Top_0.dds", 4);
	pSkyBoxTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Assets/Image/SkyBox/SkyBox_Bottom_0.dds", 5);
	SetTexture(pSkyBoxTexture);

	CSkyBoxShader *pSkyBoxShader = new CSkyBoxShader();
	pSkyBoxShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	//pSkyBoxShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pSkyBoxShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 0, 1);
	//pSkyBoxShader->CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, m_pd3dcbGameObject, ncbElementBytes);

	pSkyBoxShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pSkyBoxTexture, 6, false);


	//SetCbvGPUDescriptorHandle(pSkyBoxShader->GetGPUCbvDescriptorStartHandle());
	SetShader(pSkyBoxShader);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	XMStoreFloat4x4(&m_pcbMappedGameObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
}

void CSkyBox::Render(ID3D12GraphicsCommandList * pd3dCommandList, CCamera * pCamera)
{
	XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbGameObjectGpuVirtualAddress = m_pd3dcbGameObject->GetGPUVirtualAddress();

	OnPrepareRender();

	
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariables(pd3dCommandList);
		m_pShader->Render(pd3dCommandList, pCamera);
	}
	
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbGameObjectGpuVirtualAddress);

	if (m_pMesh) m_pMesh->Render(pd3dCommandList);

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
		    if (m_pTexture)m_pTexture->UpdateShaderVariable(pd3dCommandList,i);
			if (m_ppMeshes[i]) m_ppMeshes[i]->Render(pd3dCommandList);
		}
	}
}
