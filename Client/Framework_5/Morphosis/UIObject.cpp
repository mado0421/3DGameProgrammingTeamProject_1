#include "stdafx.h"
#include "UIObject.h"


UIObject::UIObject()
{
}


UIObject::~UIObject()
{
}

void UIObject::Initialize(XMFLOAT4 rect)
{
	m_curSize = m_maxSize = rect;
}

ID3D12Resource * UIObject::CreateShaderVariables(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(CB_UI_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbUIObject = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbUIObject->Map(0, NULL, (void **)&m_pcbMappedUIObject);

	return(m_pd3dcbUIObject);
}

void UIObject::ReleaseShaderVariables()
{
	if (m_pd3dcbUIObject)
	{
		m_pd3dcbUIObject->Unmap(0, NULL);
		m_pd3dcbUIObject->Release();
	}
	if (m_pMaterial) m_pMaterial->ReleaseShaderVariables();
}

void UIObject::UpdateShaderVariables(ID3D12GraphicsCommandList * pd3dCommandList)
{
	XMFLOAT4 difSize;
	difSize.x = m_maxSize.x - m_curSize.x;
	difSize.y = m_maxSize.y - m_curSize.y;
	difSize.z = m_maxSize.z - m_curSize.z;
	difSize.w = m_maxSize.w - m_curSize.w;

	XMStoreFloat4x4(&m_pcbMappedUIObject->m_xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));
	XMStoreFloat4(&m_pcbMappedUIObject->m_xmf4Rect, XMLoadFloat4(&difSize));
	if (m_pMaterial) m_pcbMappedUIObject->m_nMaterial = m_pMaterial->m_nReflection;
}

void UIObject::SetRootParameter(ID3D12GraphicsCommandList * pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(RP_UI, m_d3dCbvGPUDescriptorHandle);
}

void UIObject::SetSize(int side, float val)
{
	switch (side)
	{
	default:  break;
	case SIDE::L: m_curSize.x = val; break;
	case SIDE::T: m_curSize.y = val; break;
	case SIDE::R: m_curSize.z = val; break;
	case SIDE::B: m_curSize.w = val; break;
	}
}

void UIObject::SetSize(float left, float top, float right, float bottom)
{
	m_curSize.x = left;
	m_curSize.y = top;
	m_curSize.z = right;
	m_curSize.w = bottom;
}
