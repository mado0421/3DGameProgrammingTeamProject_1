#pragma once
#define NUMSWAPCHAINBUFFER 2
#define NUMDEFAULTSTRINGCHAR 64

#include "Scene.h"
#include "Timer.h"

class Framework
{
private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	int							m_nWndClientWidth;
	int							m_nWndClientHeight;

	IDXGIFactory4				*m_pdxgiFactory				= NULL;
	IDXGISwapChain3				*m_pdxgiSwapChain			= NULL;
	ID3D12Device				*m_pd3dDevice				= NULL;

	bool						m_bMsaa4xEnable				= false;
	UINT						m_nMsaa4xQualityLevels		= 0;

	static const UINT			m_nSwapChainBuffers			= NUMSWAPCHAINBUFFER;
	UINT						m_nSwapChainBufferIndex;

	ID3D12Resource				*m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap		*m_pd3dRtvDescriptorHeap	= NULL;
	UINT						m_nRtvDescriptorIncrementSize;

	ID3D12Resource				*m_pd3dDepthStencilBuffer	= NULL;
	ID3D12DescriptorHeap		*m_pd3dDsvDescriptorHeap	= NULL;
	UINT						m_nDsvDescriptorIncrementSize;

	ID3D12CommandAllocator		*m_pd3dCommandAllocator		= NULL;
	ID3D12CommandQueue			*m_pd3dCommandQueue			= NULL;
	ID3D12GraphicsCommandList	*m_pd3dCommandList			= NULL;

	ID3D12Fence					*m_pd3dFence				= NULL;
	UINT64						m_nFenceValues[m_nSwapChainBuffers];
	HANDLE						m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug					*m_pd3dDebugController;
#endif

	GameTimer					m_GameTimer;

	Scene						**m_ppScene					= NULL;
	Scene						*m_pCurrentScene			= NULL;
	int							m_nScene					= 0;

	_TCHAR						m_pszFrameRate[NUMDEFAULTSTRINGCHAR];

	bool						Activated=false;
public:
	Framework();
	~Framework();

public:
	void Render();
	void Update(float fTimeElapsed);
	void EachFrame();

public:
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	void CreateCommandQueueAndList();

	void OnResizeBackBuffers();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput(float fTimeElapsed);
	//void AnimateObjects();	//이거 업데이트 함수로 하자
	//void FrameAdvance();	//

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void ChangeScene(int targetSceneIdx, void * subData = NULL);

};

