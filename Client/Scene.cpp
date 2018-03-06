//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildLightsAndMaterials()
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));

	m_pLights->m_xmf4GlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	// 전역 조명 낮과 밤
	m_pLights->m_pLights[0].m_bEnable = true;
	m_pLights->m_pLights[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[0].m_xmf4Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[0].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);

	// 플레이어 손전등
	m_pLights->m_pLights[1].m_bEnable = true;
	m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[1].m_fRange = 2000.0f;
	m_pLights->m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights->m_pLights[1].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights->m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[1].m_fFalloff = 8.0f;
	m_pLights->m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights->m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));

	// 미로 조명

	for (int j = 0; j < 5; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_pLights->m_pLights[j*4+i+2].m_bEnable = false;
			m_pLights->m_pLights[j*4+i+2].m_nType = i*j % 2 + 1;
			m_pLights->m_pLights[j*4+i+2].m_fRange = 1000.0f;
			m_pLights->m_pLights[j*4+i+2].m_xmf4Ambient = XMFLOAT4(rand()%100*0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 1.0f);
			m_pLights->m_pLights[j*4+i+2].m_xmf4Diffuse = XMFLOAT4(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 1.0f);
			m_pLights->m_pLights[j*4+i+2].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
			m_pLights->m_pLights[j * 4 + i + 2].m_xmf3Position = XMFLOAT3(150 + j * 300, 300, 150 + i * 300);
			m_pLights->m_pLights[j*4+i+2].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
			m_pLights->m_pLights[j*4+i+2].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
			m_pLights->m_pLights[j*4+i+2].m_fFalloff = 8.0f;
			m_pLights->m_pLights[j*4+i+2].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
			m_pLights->m_pLights[j*4+i+2].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
		}
	}
	

	m_pMaterials = new MATERIALS;
	::ZeroMemory(m_pMaterials, sizeof(MATERIALS));

	m_pMaterials->m_pReflections[0] = { XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[1] = { XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[2] = { XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 15.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[3] = { XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 20.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[4] = { XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 25.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[5] = { XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 30.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[6] = { XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 35.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[7] = { XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 40.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
}
#define _WITH_TERRAIN_PARTITION
void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_nMazeShader = 1;
	m_pMazeShader = new CMazeObjectShader;
	m_pMazeShader[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pMazeShader[0].BuildObjects(pd3dDevice, pd3dCommandList);

	
	m_pMazeObjects = m_pMazeShader[0].GetObjectsPointer();
	m_nMazeObjects = m_pMazeShader[0].GetObjectsCount();

	m_nShaders = 1;
	m_pShaders = new CObjectsShader;
	m_pShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pShaders[0].BuildObjects(pd3dDevice, pd3dCommandList);

	m_pObjects = m_pShaders[0].GetObjectsPointer();
	m_nObjects = m_pShaders[0].GetObjectsCount();


	//m_nBillBoardShader = 1;
	//m_pBillBoardShader = new CBillBoardShader;
	//m_pBillBoardShader[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	//m_pBillBoardShader[0].BuildObjects(pd3dDevice, pd3dCommandList);

	

	BuildLightsAndMaterials();

	//지형을 확대할 스케일 벡터이다. x-축과 z-축은 32배, y-축은 2배 확대한다.
	XMFLOAT3 xmf3Scale(32.0f, 2.0f, 32.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.2f, 0.0f, 0.0f);

#ifdef _WITH_TERRAIN_PARTITION
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList,
		m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/HeightMapC.raw"), 257, 257, 17,
		17, xmf3Scale, xmf4Color);
#else
	//지형을 하나의 격자 메쉬(257x257)로 생성한다.
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList,
		m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/HeightMap.raw"), 257, 257, 257,
		257, xmf3Scale, xmf4Color);
#endif
	CTerrainObject* p;
	int n_CTerrainObject = m_pShaders[0].GetObjectsCount();
	for (int i = 0; i < n_CTerrainObject; ++i){
		p = (CTerrainObject*)m_pShaders[0].GetObjectsPointer()[i];
		p->SetObjectUpdatedContext(m_pTerrain);
	}

	/*CBillBoardObject *b;
	int m_nBillBoardShader = m_pBillBoardShader[0].GetObjectsCount();
	for (int i = 0; i < m_nBillBoardShader; ++i) {
		b = (CBillBoardObject*)m_pBillBoardShader[0].GetObjectsPointer()[i];
		b->SetObjectUpdatedContext(m_pTerrain);
	}*/

	// OFFSET +2 MazeObjectCount == 8 not 6
	// index 0 - 7 Real Object Index 0 - 5
	for (int i = 0; i <= m_pMazeShader[0].GetObjectsCount()+1; ++i)
	{
		p = (CTerrainObject*)m_pMazeShader[0].GetObjectsPointer()[i];
		p->SetObjectUpdatedContext(m_pTerrain);
	}
	m_pMazeObjects[m_nMazeObjects]->Animate(0);
	m_pMazeObjects[m_nMazeObjects + 1]->Animate(0);

	//m_pSkyBox = new CSkyBox(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_pShaders)
	{
		for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseShaderVariables();
		for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseObjects();
		delete[] m_pShaders;
	}

	if (m_pMazeShader)
	{
		for (int i = 0; i < m_nMazeShader; i++) m_pMazeShader[i].ReleaseShaderVariables();
		for (int i = 0; i < m_nMazeShader; i++) m_pMazeShader[i].ReleaseObjects();
		delete[] m_pMazeShader;
	}

	ReleaseShaderVariables();

	if (m_pTerrain) delete m_pTerrain;
	if (m_pLights) delete m_pLights;
	if (m_pMaterials) delete m_pMaterials;
	//if (m_pSkyBox)delete m_pSkyBox;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;


	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[4];
	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 0; // Texture;
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 1;
	pd3dDescriptorRanges[1].BaseShaderRegister = 1; // SkyBox;
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 2; // terrain;
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[3].NumDescriptors = 1;
	pd3dDescriptorRanges[3].BaseShaderRegister = 3; // terrainDetail;
	pd3dDescriptorRanges[3].RegisterSpace = 0;
	pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER pd3dRootParameters[9];
	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 0; //Player
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[1].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[1].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 2; //GameObject
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[3].Descriptor.ShaderRegister = 3; //Materials
	pd3dRootParameters[3].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[4].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[4].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0];
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;		// SkyBox
	pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1];
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;		// Terrain
	pd3dRootParameters[7].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[2];
	pd3dRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[8].DescriptorTable.NumDescriptorRanges = 1;		// TerrainDetail
	pd3dRootParameters[8].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[3];
	pd3dRootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//	StaticSampler
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDescs[2];
	::ZeroMemory(&d3dSamplerDescs, sizeof(D3D12_STATIC_SAMPLER_DESC)*2);
	d3dSamplerDescs[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDescs[0].MipLODBias = 0;
	d3dSamplerDescs[0].MaxAnisotropy = 1;
	d3dSamplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDescs[0].MinLOD = 0;
	d3dSamplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDescs[0].ShaderRegister = 0;
	d3dSamplerDescs[0].RegisterSpace = 0;
	d3dSamplerDescs[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	d3dSamplerDescs[1].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDescs[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDescs[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDescs[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	d3dSamplerDescs[1].MipLODBias = 0;
	d3dSamplerDescs[1].MaxAnisotropy = 1;
	d3dSamplerDescs[1].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDescs[1].MinLOD = 0;
	d3dSamplerDescs[1].MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDescs[1].ShaderRegister = 1;
	d3dSamplerDescs[1].RegisterSpace = 0;
	d3dSamplerDescs[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 2;
	d3dRootSignatureDesc.pStaticSamplers = d3dSamplerDescs;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);

	UINT ncbMaterialBytes = ((sizeof(MATERIALS) + 255) & ~255); //256의 배수
	m_pd3dcbMaterials = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
	
	//m_pTerrain->m_pShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);


}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights, m_pLights, sizeof(LIGHTS));
	::memcpy(m_pcbMappedMaterials, m_pMaterials, sizeof(MATERIALS));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
	if (m_pd3dcbMaterials)
	{
		m_pd3dcbMaterials->Unmap(0, NULL);
		m_pd3dcbMaterials->Release();
	}
	//m_pTerrain->ReleaseShaderVariables();
}

void CScene::SetCameraContext(CCamera * pCamera)
{
	/*if (pCamera)
	{
		CBillBoardObject *b;
		int m_nBillBoardShader = m_pBillBoardShader[0].GetObjectsCount();
		for (int i = 0; i < m_nBillBoardShader; ++i) {
			b = (CBillBoardObject*)m_pBillBoardShader[0].GetObjectsPointer()[i];
			b->SetCameraUpdateContext(pCamera);
		}

	}*/
}

void CScene::ReleaseUploadBuffers()
{
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
	for (int i = 0; i < m_nShaders; i++) m_pShaders[i].ReleaseUploadBuffers();
	for (int i = 0; i < m_nMazeShader; i++) m_pMazeShader[i].ReleaseUploadBuffers();
	//for (int i = 0; i < m_nBillBoardShader; i++) m_pBillBoardShader[i].ReleaseUploadBuffers();
	//if (m_pSkyBox)m_pSkyBox->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

XMFLOAT3 NightDirection(1.0f, 1.0f, 0.0f), DayDirection(1.0f, 0.0f, 0.0f);
bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'N':
			m_pLights->m_pLights[0].m_xmf3Direction = NightDirection;
			m_pLights->m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f);
			break;
		case 'D':
			m_pLights->m_pLights[0].m_xmf3Direction = DayDirection;
			m_pLights->m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
			break;
		case 'L':
			m_pLights->m_pLights[1].m_bEnable = 1 - m_pLights->m_pLights[1].m_bEnable;
			break;
		case 'T':
			m_pLights->m_pLights[0].m_bEnable = 1 - m_pLights->m_pLights[0].m_bEnable;
			break;
		case 'M':
			m_mazeLight=1-m_mazeLight;
			for (int i = 2; i < MAZELIGHTS_END; ++i)
				m_pLights->m_pLights[i].m_bEnable = 1 - m_pLights->m_pLights[i].m_bEnable;
			break;
		}
	}
	case WM_KEYUP:
		switch (wParam)
		{
			break;
		}
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_pPlayer->Animate(fTimeElapsed);


	// 미로 속 체크 조명 토글
	if (m_pMazeObjects[m_nMazeObjects+1]->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB))
	{
		if (m_mazeLight == false) {
			m_mazeLight = true;
			for (int i = 2; i < MAZELIGHTS_END; ++i)
				m_pLights->m_pLights[i].m_bEnable = true;
		}
	}
	else
	{
		if (m_mazeLight)
		{
			m_mazeLight = false;
			for (int i = 2; i < MAZELIGHTS_END; ++i)
				m_pLights->m_pLights[i].m_bEnable = false;
		}
	}

	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].AnimateObjects(fTimeElapsed);
	}

	//for (int i = 0; i < m_nBillBoardShader; i++)
	//{
	//	m_pBillBoardShader[i].AnimateObjects(fTimeElapsed);
	//}

	// 오브젝트간 충돌
	for (int i = 0; i < m_nObjects-1; ++i)
	{
		for (int j = i; j < m_nObjects; ++j)
		{
			if (m_pObjects[i]->m_xmOOBB.Intersects(m_pObjects[j]->m_xmOOBB))
			{
				XMFLOAT3 tmp = m_pObjects[i]->m_xmf3MovingDirection;
				m_pObjects[i]->m_xmf3MovingDirection = m_pObjects[j]->m_xmf3MovingDirection;
				m_pObjects[j]->m_xmf3MovingDirection = tmp;
			}	
		}
	}

	for (int i = 0; i < m_nMazeShader; i++)
	{
		m_pMazeShader[i].AnimateObjects(fTimeElapsed);
	}
	
	//	플레이어 벽 충돌
	if (m_pMazeObjects[1]->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB)){
		m_pPlayer->Move(Vector3::ScalarProduct(m_pPlayer->m_xmf3MovingDirection, -100));
	}
	else if (m_pMazeObjects[2]->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB)) {
		m_pPlayer->Move(Vector3::ScalarProduct(m_pPlayer->m_xmf3MovingDirection, -100));
	}
	else if (m_pMazeObjects[3]->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB)){
		m_pPlayer->Move(Vector3::ScalarProduct(m_pPlayer->m_xmf3MovingDirection, -100));
	}
	else if (m_pMazeObjects[6]->m_xmOOBB.Intersects(m_pPlayer->m_xmOOBB)){
		m_pPlayer->Move(Vector3::ScalarProduct(m_pPlayer->m_xmf3MovingDirection, -100));
	}
	

	//	오브젝트 충돌
	for (int i = 0; i < m_nObjects; ++i)
	{
		for (int j = 0; j < m_nMazeObjects-2; ++j) {
			if (m_pMazeObjects[j]->m_xmOOBB.Intersects(m_pObjects[i]->m_xmOOBB))
			{
				m_pObjects[i]->ReverseMovingDirection();
			}
		}
		if (m_pPlayer->m_xmOOBB.Intersects(m_pObjects[i]->m_xmOOBB))
		{
			XMFLOAT3 playerPosition = m_pPlayer->GetPosition();
			XMFLOAT3 ObjectPosition = m_pObjects[i]->GetPosition();
			XMFLOAT3 reflectDirection = Vector3::Subtract(ObjectPosition, playerPosition);
			reflectDirection=Vector3::Normalize(reflectDirection);
			m_pObjects[i]->SetMovingDirection(reflectDirection);
		}
	}

	//	손전등
	if (m_pLights)
	{
		m_pLights->m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights->m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(4, d3dcbLightsGpuVirtualAddress); //Lights

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(3, d3dcbMaterialsGpuVirtualAddress);

	//if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);

	if (m_pTerrain) 
	{
		m_pTerrain->Render(pd3dCommandList, pCamera);
	}

	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].Render(pd3dCommandList, pCamera);
	}

	for (int i = 0; i < m_nMazeShader; i++)
	{
		m_pMazeShader[i].Render(pd3dCommandList, pCamera);
	}
	
	
	//for (int i = 0; i < m_nBillBoardShader; i++)
	//{
	//	m_pBillBoardShader->Render(pd3dCommandList, pCamera);
	//}

}

