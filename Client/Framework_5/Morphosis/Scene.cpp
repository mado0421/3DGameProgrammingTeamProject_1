#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Initialize(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, HWND hWnd)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_pCamera = new CCamera();
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_hWnd = hWnd;
	SetCapture(m_hWnd);
	GetCursorPos(&m_ptOldCursorPos); 
	ShowCursor(false);
	m_bCurCursorMoveableState = false;
}

void Scene::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	m_pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	m_pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->Render(pd3dCommandList, m_pCamera);
	
}





void Scene::Update(float fTimeElapsed)
{
	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->Update(fTimeElapsed);
}

bool Scene::ProcessInput(UCHAR * pKeysBuffer, float fTimeElapsed)
{
	return(false);
}

bool Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

ID3D12RootSignature * Scene::CreateGraphicsRootSignature(ID3D12Device * pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[RDR_NUMOFRANGE];

	pd3dDescriptorRanges[RDR_OBJECT].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	pd3dDescriptorRanges[RDR_OBJECT].NumDescriptors = 1;
	pd3dDescriptorRanges[RDR_OBJECT].BaseShaderRegister = RP_OBJECT; //Game Objects
	pd3dDescriptorRanges[RDR_OBJECT].RegisterSpace = 0;
	pd3dDescriptorRanges[RDR_OBJECT].OffsetInDescriptorsFromTableStart = 0;

	pd3dDescriptorRanges[RDR_TEXTURE].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[RDR_TEXTURE].NumDescriptors = 1;
	pd3dDescriptorRanges[RDR_TEXTURE].BaseShaderRegister = RP_TEXTURE; //Texture
	pd3dDescriptorRanges[RDR_TEXTURE].RegisterSpace = 0;
	pd3dDescriptorRanges[RDR_TEXTURE].OffsetInDescriptorsFromTableStart = 0;

	pd3dDescriptorRanges[RDR_UI].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	pd3dDescriptorRanges[RDR_UI].NumDescriptors = 1;
	pd3dDescriptorRanges[RDR_UI].BaseShaderRegister = RP_UI; //UI
	pd3dDescriptorRanges[RDR_UI].RegisterSpace = 0;
	pd3dDescriptorRanges[RDR_UI].OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER pd3dRootParameters[RP_NUMOFPARAMETER];

	pd3dRootParameters[RP_CAMERA].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[RP_CAMERA].Descriptor.ShaderRegister = RP_CAMERA; //CCamera
	pd3dRootParameters[RP_CAMERA].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[RP_CAMERA].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[RP_OBJECT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[RP_OBJECT].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[RP_OBJECT].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[RDR_OBJECT]; //Game Objects
	pd3dRootParameters[RP_OBJECT].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[RP_MATERIAL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[RP_MATERIAL].Descriptor.ShaderRegister = RP_MATERIAL; //Materials
	pd3dRootParameters[RP_MATERIAL].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[RP_MATERIAL].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[RP_LIGHT].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[RP_LIGHT].Descriptor.ShaderRegister = RP_LIGHT; //Lights
	pd3dRootParameters[RP_LIGHT].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[RP_LIGHT].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[RP_TEXTURE].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[RP_TEXTURE].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[RP_TEXTURE].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[RDR_TEXTURE]; //Texture
	pd3dRootParameters[RP_TEXTURE].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[RP_VERTEX].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	pd3dRootParameters[RP_VERTEX].Descriptor.ShaderRegister = RP_VERTEX; //t5
	pd3dRootParameters[RP_VERTEX].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[RP_VERTEX].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[RP_UV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	pd3dRootParameters[RP_UV].Descriptor.ShaderRegister = RP_UV; //t6
	pd3dRootParameters[RP_UV].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[RP_UV].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	pd3dRootParameters[RP_UI].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[RP_UI].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[RP_UI].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[RDR_UI]; //Game UI
	pd3dRootParameters[RP_UI].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	/*
	Root Signiature 추가 필요
	*/
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	HRESULT isSuccess = D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pd3dSignatureBlob, &pd3dErrorBlob);
//	assert(isSuccess == S_OK);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void Scene::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
}

void Scene::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	for (int i = 0; i < m_nShaders; ++i) m_ppShaders[i]->UpdateShaderVariables(pd3dCommandList);
}

void Scene::ReleaseShaderVariables()
{
}

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, HWND hWnd)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_pCamera = new BoardCamera();
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_hWnd = hWnd;	
	SetCapture(m_hWnd);
	GetCursorPos(&m_ptOldCursorPos);
	ShowCursor(false);
	m_bCurCursorMoveableState = false;
}

void LoadingScene::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	Scene::Render(pd3dCommandList);
}

GroundScene::GroundScene()
{
}

GroundScene::~GroundScene()
{
}

void GroundScene::BuildLights()
{
	m_pLightsArr = new LIGHTS;
	::ZeroMemory(m_pLightsArr, sizeof(LIGHTS));

	m_pLightsArr->m_xmf4GlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	m_pLightsArr->m_pLights[0].m_bEnable = true;
	m_pLightsArr->m_pLights[0].m_nType = POINT_LIGHT;
	m_pLightsArr->m_pLights[0].m_fRange = 1000.0f;
	m_pLightsArr->m_pLights[0].m_xmf4Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLightsArr->m_pLights[0].m_xmf4Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLightsArr->m_pLights[0].m_xmf4Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLightsArr->m_pLights[0].m_xmf3Position = XMFLOAT3(0.0f, 100.0f, 0.0f);
	m_pLightsArr->m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLightsArr->m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	//m_pLightsArr->m_pLights[1].m_bEnable = true;
	//m_pLightsArr->m_pLights[1].m_nType = SPOT_LIGHT;
	//m_pLightsArr->m_pLights[1].m_fRange = 100.0f;
	//m_pLightsArr->m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pLightsArr->m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);
	//m_pLightsArr->m_pLights[1].m_xmf4Specular = XMFLOAT4(0.58f, 0.58f, 0.58f, 0.0f);
	//m_pLightsArr->m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	//m_pLightsArr->m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//m_pLightsArr->m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pLightsArr->m_pLights[1].m_fFalloff = 8.0f;
	//m_pLightsArr->m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	//m_pLightsArr->m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	//m_pLightsArr->m_pLights[2].m_bEnable = true;
	//m_pLightsArr->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	//m_pLightsArr->m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	//m_pLightsArr->m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
	//m_pLightsArr->m_pLights[2].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pLightsArr->m_pLights[2].m_xmf3Direction = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	//m_pLightsArr->m_pLights[3].m_bEnable = true;
	//m_pLightsArr->m_pLights[3].m_nType = SPOT_LIGHT;
	//m_pLightsArr->m_pLights[3].m_fRange = 60.0f;
	//m_pLightsArr->m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pLightsArr->m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_pLightsArr->m_pLights[3].m_xmf4Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	//m_pLightsArr->m_pLights[3].m_xmf3Position = XMFLOAT3(-150.0f, 30.0f, 30.0f);
	//m_pLightsArr->m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	//m_pLightsArr->m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pLightsArr->m_pLights[3].m_fFalloff = 8.0f;
	//m_pLightsArr->m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	//m_pLightsArr->m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
	//m_pLightsArr->m_pLights[4].m_bEnable = true;
	//m_pLightsArr->m_pLights[4].m_nType = DIRECTIONAL_LIGHT;
	//m_pLightsArr->m_pLights[4].m_xmf4Ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	//m_pLightsArr->m_pLights[4].m_xmf4Diffuse = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
	//m_pLightsArr->m_pLights[4].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pLightsArr->m_pLights[4].m_xmf3Direction = XMFLOAT3(1.0f, 1.0f, 1.0f);

}

void GroundScene::BuildMaterials()
{
	m_pMaterials = new MATERIALS;
	::ZeroMemory(m_pMaterials, sizeof(MATERIALS));

	m_pMaterials->m_pReflections[0] = { XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[1] = { XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[2] = { XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 15.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[3] = { XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 20.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[4] = { XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 25.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[5] = { XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 30.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[6] = { XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 35.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	m_pMaterials->m_pReflections[7] = { XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 40.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };

	m_nMaterials = 8;
}

void GroundScene::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	HRESULT result = m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);

	UINT ncbMaterialBytes = ((sizeof(MATERIALS) + 255) & ~255); //256의 배수
	m_pd3dcbMaterials = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbMaterialBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbMaterials->Map(0, NULL, (void **)&m_pcbMappedMaterials);
}

void GroundScene::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	::memcpy(m_pcbMappedLights, m_pLightsArr, sizeof(LIGHTS));
	::memcpy(m_pcbMappedMaterials, m_pMaterials, sizeof(MATERIALS));
	for(int i =0;i < m_nShaders; ++i)
		m_ppShaders[i]->UpdateShaderVariables(pd3dCommandList);
}

void GroundScene::ReleaseShaderVariables()
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
}

void GroundScene::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, HWND hWnd)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_nShaders = GShaders::NumOfGShader;
	m_ppShaders = new Shader*[m_nShaders];

	GCharacterShader *pCharShader = new GCharacterShader();
	pCharShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pCharShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_Character] = pCharShader;
	pGCS = pCharShader;
	m_pPlayer = pCharShader->GetTargetPlayer(0);
	
	
	m_sock =socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	addr.sin_addr.s_addr = inet_addr("210.99.149.149");
	int retval = WSAConnect(m_sock, (sockaddr *)&addr, sizeof(addr), NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR)
	{
		CYH::ErrorDisplay("connect");
	}
	m_pPlayer->m_sock = m_sock;
	pCharShader->m_sock = m_sock;
	sendPlayerInfo(m_pPlayer);
	thread* p = new thread{ recvFunc,this,pCharShader };


	GCollideObjectShader *pCollObjShader = new GCollideObjectShader();
	pCollObjShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pCollObjShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_CollObj] = pCollObjShader;

	GDecorateObjectShader *pDecoObjShader = new GDecorateObjectShader();
	pDecoObjShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pDecoObjShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_DecoObj] = pDecoObjShader;

	GlevelShader *pLevelShader = new GlevelShader();
	pLevelShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pLevelShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_Level] = pLevelShader;

	GSkyboxShader *pSkyboxShader = new GSkyboxShader();
	pSkyboxShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pSkyboxShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_Skybox] = pSkyboxShader;

	GParticleShader *pParticleShader = new GParticleShader();
	pParticleShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pParticleShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_Particle] = pParticleShader;

	GUIShader *pUIShader = new GUIShader();
	pUIShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pUIShader->BuildObjects(pd3dDevice, pd3dCommandList);
	m_ppShaders[GShaders::_UI] = pUIShader;
	pGUIS = pUIShader;

	m_pCamera = new FollowCamera();
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	
	m_pCamera->SetTarget(m_pPlayer);

	BuildLights();
	BuildMaterials();
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_hWnd = hWnd;
	SetCapture(m_hWnd);
	GetCursorPos(&m_ptOldCursorPos);
	ShowCursor(false);
	m_bCurCursorMoveableState = false;

}

void GroundScene::Render(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	m_pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	m_pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(RP_LIGHT, d3dcbLightsGpuVirtualAddress); //Lights
	D3D12_GPU_VIRTUAL_ADDRESS d3dcbMaterialsGpuVirtualAddress = m_pd3dcbMaterials->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(RP_MATERIAL, d3dcbMaterialsGpuVirtualAddress); //Materials

	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->Render(pd3dCommandList, m_pCamera);


}

void GroundScene::Update(float fTimeElapsed)
{
	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->Update(fTimeElapsed);
	XMFLOAT3 pos = m_pPlayer->GetPosition();
	pos.y += 40.0;
	m_pCamera->Update(pos, fTimeElapsed);
	m_pCamera->RegenerateViewMatrix();
//	pGUIS->SetLook(m_pCamera->GetLook());
}

bool GroundScene::ProcessInput(UCHAR * pKeysBuffer, float fTimeElapsed)
{
	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	DWORD dwDirection = 0;

	if (GetCapture() == m_hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}

	if (pKeysBuffer[KeyCode::_W] & 0xF0) dwDirection |= DIR_FORWARD;
	if (pKeysBuffer[KeyCode::_S] & 0xF0) dwDirection |= DIR_BACKWARD;
	if (pKeysBuffer[KeyCode::_A] & 0xF0) dwDirection |= DIR_LEFT;
	if (pKeysBuffer[KeyCode::_D] & 0xF0) dwDirection |= DIR_RIGHT;
	if (pKeysBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
	if (pKeysBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
	//if (0 != dwDirection) { 
	//	m_pPlayer->Move(dwDirection, 100.0f * fTimeElapsed, true); 
	//	m_pPlayer->Test(); 
	//}
	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta)
		{
			//m_pPlayer->Rotate(cyDelta, -cxDelta, 0.0f);
			sendRotateInfo(m_pPlayer, cxDelta, cyDelta);
		}
		if (0 != dwDirection) {
			sendMoveInfo(m_pPlayer, dwDirection);
		}
	}


	//if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	//{
	//	if (cxDelta || cyDelta)
	//	{
	//		m_pPlayer->Rotate(cyDelta, -cxDelta, 0.0f);
	//		m_pPlayer->Test();
	//		m_pCamera->Rotate(cyDelta, -cxDelta, 0.0f);
	//	}
	//	if (dwDirection) {
	//		m_pPlayer->Move(dwDirection, 100.0f * fTimeElapsed, true);
	//		m_pPlayer->Test();
	//	}
	//}

	// press Mouse Left Button
	if (pKeysBuffer[VK_LBUTTON] & 0xF0)
	{
		//pGCS->AddBullet(0);
		sendAddBulletPacket(m_pPlayer);
	}

	// press Mouse Right Button
	if (pKeysBuffer[VK_RBUTTON] & 0xF0)
	{
		//pGCS->AddSkillProjectile(0, 0);
		sendAddSkillProjectile(m_pPlayer, 0);
	}

	if (pKeysBuffer[VK_LSHIFT] & 0xF0)
	{
		//pGCS->AddSkillProjectile(0, 0);
		sendAddSkillProjectile(m_pPlayer, 1);
	}

	if (pKeysBuffer[KeyCode::_Q] & 0xF0)
	{
		//pGCS->AddSkillProjectile(0, 0);
		sendAddSkillProjectile(m_pPlayer, 2);
	}

	//for Debug
	if (pKeysBuffer[VK_SPACE] & 0xF0) {
		ChangeCursorMoveableState();
	}

	if (pKeysBuffer[VK_RETURN] & 0xF0) {
		m_pPlayer->Test();
	}

	return true;
}

void GroundScene::sendPlayerInfo(Character * p)
{
	cs_packet_addplayer* addPacket;
	addPacket = new cs_packet_addplayer;
	
	addPacket->weapon = p->weapon = 1;
	addPacket->main1 = p->m_number[0].main = 0;
	addPacket->sub11 = p->m_number[0].sub1 = 0;
	addPacket->sub12 = p->m_number[0].sub2 = 0;
	addPacket->main2 = p->m_number[1].main = 1;
	addPacket->sub21 = p->m_number[1].sub1 = 1;
	addPacket->sub22 = p->m_number[1].sub2 = 1;
	addPacket->main3 = p->m_number[2].main = 2;
	addPacket->sub31 = p->m_number[2].sub1 = 2;
	addPacket->sub32 = p->m_number[2].sub2 = 2;
	addPacket->posX = 0;
	addPacket->posY = 0;
	addPacket->posZ = 0;
	addPacket->size = sizeof(cs_packet_addplayer);
	addPacket->type = csKIND::addPlayer;
	int retval = send(p->m_sock, (char*)addPacket, sizeof(cs_packet_addplayer), 0);
}

void GroundScene::sendMoveInfo(Character * p, DWORD dwDirection)
{
	if (p->m_direction == dwDirection)
		return;
	cs_packet_move* movePacket;
	movePacket = new cs_packet_move;
	movePacket->direction = dwDirection;
	movePacket->size = sizeof(cs_packet_move);
	movePacket->type = csKIND::move;
	int retval = send(p->m_sock, (char*)movePacket, sizeof(cs_packet_move), 0);
}

void GroundScene::sendRotateInfo(Character * p, float cxDelta, float cyDelta)
{
	static float scxDelta=0.0f;
	static float scyDelta=0.0f;
	scxDelta += cxDelta;
	scyDelta += cyDelta;
	if (scxDelta*scxDelta + scyDelta * scyDelta > 25.0f)
	{
		cs_packet_rotate* rotatePacket;
 		rotatePacket = new cs_packet_rotate;
		rotatePacket->cxDelta = scxDelta;
		rotatePacket->cyDelta = scyDelta;
		rotatePacket->size = sizeof(cs_packet_rotate);
		rotatePacket->type = csKIND::rotate;
		rotatePacket->posX = p->m_xmf4x4World._41;
		rotatePacket->posY = p->m_xmf4x4World._42;
		rotatePacket->posZ = p->m_xmf4x4World._43;
		scxDelta = 0.0f;
		scyDelta = 0.0f;
		int retval = send(p->m_sock, (char*)rotatePacket, sizeof(cs_packet_rotate), 0);
	}
}

void GroundScene::sendAddBulletPacket(Character * p)
{
	static int idx = 0;
	cs_packet_addBullet* addBulletPacket;
	addBulletPacket = new cs_packet_addBullet;
	addBulletPacket->size = sizeof(cs_packet_addBullet);
	addBulletPacket->bulletID = idx++;
	addBulletPacket->type = csKIND::addBullet;
	int retval = send(p->m_sock, (char*)addBulletPacket, sizeof(cs_packet_addBullet), 0);
}

void GroundScene::sendAddSkillProjectile(Character * p, int idx)
{
	static int index = 0;
	cs_packet_addSkill* addSkillPacket;
	addSkillPacket = new cs_packet_addSkill;
	addSkillPacket->size = sizeof(cs_packet_addSkill);
	addSkillPacket->skillID = index++;
	addSkillPacket->idx = idx;
	addSkillPacket->type = csKIND::addSkill;
	int retval = send(p->m_sock, (char*)addSkillPacket, sizeof(cs_packet_addSkill), 0);
}

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, HWND hWnd)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_nShaders = 1;
	m_ppShaders = new Shader*[m_nShaders];

	//	ID3D12Resource *pd3dcbResource = Object::CreateShaderVariables(pd3dDevice, pd3dCommandList);

	LUIShader *pUIShader = new LUIShader();
	pUIShader->Initialize(pd3dDevice, m_pd3dGraphicsRootSignature);
	pUIShader->BuildObjects(pd3dDevice, pd3dCommandList);
	////pUIShader->
	//pUIShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//pUIShader->CreateCbvAndSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1);
	////	pUIShader->CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, );
	////	pUIShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTexture, 5, true);
	m_ppShaders[0] = pUIShader;

	m_pCamera = new BoardCamera();
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_hWnd = hWnd;
	SetCapture(m_hWnd);
	GetCursorPos(&m_ptOldCursorPos);
	ShowCursor(false);
	m_bCurCursorMoveableState = false;
}

EnterRoomScene::EnterRoomScene()
{
}

EnterRoomScene::~EnterRoomScene()
{
}

MatchingScene::MatchingScene()
{
}

MatchingScene::~MatchingScene()
{
}

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void recvFunc(GroundScene* p, GCharacterShader* s)
{
	char buf[255];
	BYTE length;
	int target;
	sc_packet_addplayer addPacket;
	sc_packet_move movePacket;
	sc_packet_rotate rotatePacket;
	sc_packet_addBullet addBulletPacket;
	sc_packet_addSkill addSkillPacket;
	sc_packet_hitBullet hitBulletPacket;
	Character* other[7];
	CYH::recvn(p->m_sock, (char*)&length, 1, 0);
	CYH::recvn(p->m_sock, buf, length - 1, 0);
	memcpy((char*)&addPacket + 1, buf, sizeof(sc_packet_addplayer) - 1);
	s->GetTargetPlayer(0)->m_myID = addPacket.id;
	s->GetTargetPlayer(0)->m_active = true;
	s->GetTargetPlayer(0)->m_team = addPacket.id % 2;
	s->GetTargetPlayer(0)->m_xmf4x4World._41 = addPacket.posX;
	s->GetTargetPlayer(0)->m_xmf4x4World._42 = addPacket.posY;
	s->GetTargetPlayer(0)->m_xmf4x4World._43 = addPacket.posZ;
	for (int i = 0; i < 7; ++i)
	{
		other[i] = s->GetTargetPlayer(i + 1);
	}
	while (1)
	{
		//한 바이트 받고, 그만큼 나머지받기
		CYH::recvn(p->m_sock, (char*)&length, 1, 0);
		CYH::recvn(p->m_sock, buf, length - 1, 0);
		switch (buf[0])
		{
		case scKIND::addPlayer:
			memcpy((char*)&addPacket + 1, buf, sizeof(sc_packet_addplayer) - 1);
			for (int i = 0; i < 7; ++i)
			{
				if ((other[i]->m_isconnected == false) && (addPacket.id != s->GetTargetPlayer(0)->m_myID))
				{
					other[i]->weapon = addPacket.weapon;
					other[i]->m_number[0].main = addPacket.main1;
					other[i]->m_number[0].sub1 = addPacket.sub11;
					other[i]->m_number[0].sub2 = addPacket.sub12;
					other[i]->m_number[1].main = addPacket.main2;
					other[i]->m_number[1].sub1 = addPacket.sub21;
					other[i]->m_number[1].sub2 = addPacket.sub22;
					other[i]->m_number[2].main = addPacket.main3;
					other[i]->m_number[2].sub1 = addPacket.sub31;
					other[i]->m_number[2].sub2 = addPacket.sub32;
					other[i]->m_myID = addPacket.id;
					other[i]->m_isconnected = true;
					other[i]->m_xmf4x4World._41 = addPacket.posX;
					other[i]->m_xmf4x4World._42 = addPacket.posY;
					other[i]->m_xmf4x4World._43 = addPacket.posZ;
					other[i]->m_team = addPacket.id % 2;
					other[i]->m_active = true;
					break;
				}
			}
			break;
		case scKIND::move:
			memcpy((char*)&movePacket + 1, buf, sizeof(sc_packet_move) - 1);
			for (int i = 0; i < 8; ++i)
			{
				if (movePacket.id == s->GetTargetPlayer(i)->m_myID)
				{
					s->GetTargetPlayer(i)->m_direction = movePacket.direction;
					break;
				}
			}
			break;
		case scKIND::rotate:
			memcpy((char*)&rotatePacket + 1, buf, sizeof(sc_packet_rotate) - 1);
			for (int i = 0; i < 8; ++i)
			{
				if (rotatePacket.id == s->GetTargetPlayer(i)->m_myID)
				{
					s->GetTargetPlayer(i)->m_cxDelta = rotatePacket.cxDelta/50;
					s->GetTargetPlayer(i)->m_cyDelta = rotatePacket.cyDelta/50;
					s->GetTargetPlayer(i)->m_xmf4x4World._41 = rotatePacket.posX;
					s->GetTargetPlayer(i)->m_xmf4x4World._42 = rotatePacket.posY;
					s->GetTargetPlayer(i)->m_xmf4x4World._43 = rotatePacket.posZ;
					break;
				}
			}
			break;
		case scKIND::addBullet:
			memcpy((char*)&addBulletPacket + 1, buf, sizeof(sc_packet_addBullet) - 1);
			for (int i = 0; i < 8; ++i)
			{
				if (addBulletPacket.id == s->GetTargetPlayer(i)->m_myID)
				{
					s->AddBullet(i,addBulletPacket.bulletID);
					break;
				}
			}
			break;
		case scKIND::addSkill:
			memcpy((char*)&addSkillPacket + 1, buf, sizeof(sc_packet_addSkill) - 1);
			for (int i = 0; i < 8; ++i)
			{
				if (addSkillPacket.id == s->GetTargetPlayer(i)->m_myID)
				{
					s->AddSkillProjectile(i, addSkillPacket.idx,addSkillPacket.skillID);
					break;
				}
			}
			break;
		case scKIND::hitBullet:
			memcpy((char*)&hitBulletPacket + 1, buf, sizeof(sc_packet_hitBullet) - 1);
			//s->deleteBullet(hitBulletPacket.ownerID, hitBulletPacket.bulletID);
			for (int i = 0; i < 8; ++i)
			{
				if (hitBulletPacket.ownerID == s->GetTargetPlayer(i)->m_myID)
				{
					for (int j = 0; j < BulletPC; ++j)
					{
						if (s->m_ppBullets[(BulletPC*i) + j]->m_id == hitBulletPacket.bulletID)
						{
							if (s->m_ppBullets[(BulletPC*i) + j]->m_active == false)
								return;
							else
							{
								s->m_ppBullets[(BulletPC*i) + j]->m_active = false;
								break;
							}
						}
					}
				}
				if (hitBulletPacket.victimID == s->GetTargetPlayer(i)->m_myID)
				{
					s->GetTargetPlayer(i)->Damaged(10);
					break;
				}
			}
			break;
		}
	}
}