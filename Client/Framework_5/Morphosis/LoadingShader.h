#pragma once
#include "Shader.h"

class LUIShader : public TexturedShader
{
private:

public:
	LUIShader();
	~LUIShader();

public:
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext = NULL);
	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

};

