#pragma once
#include "Mesh.h"

struct CB_UI_INFO
{
	XMFLOAT4X4						m_xmf4x4World;
	XMFLOAT4						m_xmf4Rect;
	UINT							m_nMaterial;
};

class UIMesh : public CMeshTextured
{
private:

public:
	UIMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,
		XMFLOAT4 rect, float fDepthLevel = 0.0f);
	~UIMesh();
public:
};

class TestMesh : public CMeshIlluminatedTextured 
{
private:

public:
	TestMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	TestMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float size);
	~TestMesh();

public:

};