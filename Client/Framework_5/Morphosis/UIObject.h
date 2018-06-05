#pragma once
#include "Object.h"
#include "AdvencedMesh.h"

class UIObject : public Object
{
public:
	ID3D12Resource	*m_pd3dcbUIObject = NULL;
	CB_UI_INFO		*m_pcbMappedUIObject = NULL;
	XMFLOAT4		m_maxSize;
	XMFLOAT4		m_curSize;

public:
	UIObject();
	~UIObject();

public:
	void Initialize(XMFLOAT4 rect);

	virtual ID3D12Resource *CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void SetRootParameter(ID3D12GraphicsCommandList *pd3dCommandList);


};

