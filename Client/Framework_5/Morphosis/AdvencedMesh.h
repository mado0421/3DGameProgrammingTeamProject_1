#pragma once
#include "Mesh.h"

class UIMesh : public CMeshTextured
{
private:

public:
	UIMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,
		float left, float top, float right, float bottom,
		float fDepthLevel = 0.0f);
	~UIMesh();

public:

};

class TestMesh : public CMeshIlluminatedTextured 
{
private:

public:
	TestMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	~TestMesh();

public:

};