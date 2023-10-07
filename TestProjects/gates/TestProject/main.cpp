#include "CoreTypes.h"

#include <Windows.h>
#include <crtdbg.h>

#define SAFE_RELEASE(p) p->Release()
#define GET_IID(ppType) IID_PPV_ARGS(ppType)

#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"winmm.lib")

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		//UINT width = LOWORD(lparam);
		//UINT height = HIWORD(lparam);
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Int32 WINAPI WinMain(HINSTANCE testInstance, HINSTANCE, LPSTR, Int32)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// -------- frameRate counter ------------------------------------------------------------

	float fps = 0;
	float frameTime = 0;
	std::chrono::system_clock::time_point startTime, endTime;
	startTime = std::chrono::system_clock::now();

	// -------- create window ----------------------------------------------------------------

	HINSTANCE hInstance = {};
	HWND hwnd = {};
	WNDCLASSEX wndClass = {};
	MSG msg = {};

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WinProc;
	wndClass.lpszClassName = "HinomaruEngine";
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hIcon = nullptr;

	RegisterClassEx(&wndClass);

	RECT rect = { 0,0,800,450 };
	DWORD windowMode = WS_OVERLAPPEDWINDOW;

	AdjustWindowRect(&rect, windowMode, false);
	hwnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszClassName, windowMode,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		nullptr,
		nullptr,
		wndClass.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);


	// -------- setup d3d12 ----------------------------------------------------------------

	HRESULT hr = {};

	// デバッグレイヤーをONに
	UINT debugFlag = 0;
	ID3D12Debug* pDebugCtrler = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(GET_IID(&pDebugCtrler))))
	{
		pDebugCtrler->EnableDebugLayer();
		debugFlag |= DXGI_CREATE_FACTORY_DEBUG;
	}

	IDXGIAdapter1* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	if (!SUCCEEDED(CreateDXGIFactory2(debugFlag, GET_IID(&pFactory))))
	{
		return -1;
	}

	// 6の機能を使いたいため
	IDXGIFactory6* pFactory6 = nullptr;
	if (!SUCCEEDED(pFactory->QueryInterface(GET_IID(&pFactory6))))
	{
		SAFE_RELEASE(pFactory);
		return -1;
	}

	// 使えるGPU検索（パフォーマンスが高い順）
	std::vector<DXGI_ADAPTER_DESC1> adapterDatas;
	for (UINT adapterIndex = 0;
		SUCCEEDED(pFactory6->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, GET_IID(&pAdapter)));
		++adapterIndex)
	{
		adapterDatas.push_back(DXGI_ADAPTER_DESC1());
		DXGI_ADAPTER_DESC1& adapterData = adapterDatas.back();
		pAdapter->GetDesc1(&adapterData);

		// 一度すべてのデバイスを取得したいため作ったCOMを解放
		SAFE_RELEASE(pAdapter);
	}

	// パフォーマンスが一番高いデバイス取得
	pFactory6->EnumAdapterByLuid(adapterDatas[0].AdapterLuid, GET_IID(&pAdapter));

	ID3D12Device* pDevice = nullptr;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	// 対応している機能レベルが最大のものでデバイス作成
	for (int i = 0; i < _countof(featureLevels); ++i)
	{
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, featureLevels[i], GET_IID(&pDevice))))break;
	}

	ID3D12CommandQueue* pCmdQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	hr = pDevice->CreateCommandQueue(&cmdQueueDesc, GET_IID(&pCmdQueue));

	ID3D12CommandAllocator* pCmdAlloc = nullptr;
	if (!SUCCEEDED(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, GET_IID(&pCmdAlloc))))
	{
		SAFE_RELEASE(pAdapter);
		SAFE_RELEASE(pFactory);
		SAFE_RELEASE(pFactory6);
		SAFE_RELEASE(pCmdQueue);
		SAFE_RELEASE(pDevice);
		return -1;
	}

	ID3D12GraphicsCommandList* pCmdList = nullptr;
	hr = pDevice->CreateCommandList(cmdQueueDesc.NodeMask, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAlloc, nullptr, GET_IID(&pCmdList));

	const UInt32 FRAME_COUNT = 2;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.BufferCount = FRAME_COUNT;
	swapchainDesc.Width = rect.right - rect.left;
	swapchainDesc.Height = rect.bottom - rect.top;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.SampleDesc.Count = 1;

	IDXGISwapChain1* pSwapchain = nullptr;
	pFactory6->CreateSwapChainForHwnd(pCmdQueue, hwnd, &swapchainDesc, nullptr, nullptr, &pSwapchain);

	IDXGISwapChain3* ppSwapchain = nullptr;
	pSwapchain->QueryInterface(GET_IID(&ppSwapchain));

	UInt32 frameIndex = ppSwapchain->GetCurrentBackBufferIndex();

	ID3D12DescriptorHeap* pRtvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = FRAME_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	pDevice->CreateDescriptorHeap(&rtvHeapDesc, GET_IID(&pRtvHeap));

	ID3D12Resource* pRenderTarget[FRAME_COUNT];
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = pRtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (Int32 i = 0; i < FRAME_COUNT; ++i)
	{
		ppSwapchain->GetBuffer(i, GET_IID(&pRenderTarget[i]));
		pDevice->CreateRenderTargetView(pRenderTarget[i], nullptr, rtvHandle);
		rtvHandle.ptr += pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	ID3D12Fence* pFence = nullptr;
	UInt64 fenceValue = 0;
	pDevice->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, GET_IID(&pFence));


	while (msg.message != WM_QUIT)
	{
		endTime = std::chrono::system_clock::now();
		frameTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000.f / 1000.f;

		//// fps調整
		//const float FRAMERATE = 70.f;
		//const float WAIT_FRAMETIME = 1.f / FRAMERATE;
		//if (frameTime < WAIT_FRAMETIME)
		//{
		//	float sleepTime = (WAIT_FRAMETIME - frameTime);
		//	sleepTime *= 1000.f * 1000.f;

		//	// 標準の分解脳だと60fpsでwaitをかけても30fps程度しか出せない
		//	// マルチプラットフォームのこと考えるとtimeBeginPeriodは使いたくない...
		//	//timeBeginPeriod(1);
		//	std::this_thread::sleep_for(std::chrono::microseconds((UInt64)(sleepTime)));
		//	//timeEndPeriod(1);

		//	continue;
		//}

		startTime = endTime;
		fps = 1.0f / frameTime;

		char pStrBuf[256];
		sprintf_s(pStrBuf, "fps : %d\n", (Int32)fps);
		OutputDebugString(pStrBuf); 

		// update



		// render
		// サーフェイスをプレゼント状態からレンダーターゲットに変更
		D3D12_RESOURCE_BARRIER resBarrier = {};
		resBarrier.Transition.pResource = pRenderTarget[frameIndex];
		resBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		resBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		pCmdList->ResourceBarrier(1, &resBarrier);

		// 画面クリア
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		D3D12_CPU_DESCRIPTOR_HANDLE currentRtvHandle = pRtvHeap->GetCPUDescriptorHandleForHeapStart();
		currentRtvHandle.ptr += frameIndex * pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		pCmdList->ClearRenderTargetView(currentRtvHandle, clearColor, 0, nullptr);

		// 描画命令


		// サーフェイスをレンダーターゲットからプレゼントに変更
		resBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		resBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		pCmdList->ResourceBarrier(1, &resBarrier);

		// コマンドリスト閉鎖
		pCmdList->Close();
		ID3D12CommandList* ppCmdList[] = { pCmdList };
		pCmdQueue->ExecuteCommandLists(_countof(ppCmdList), ppCmdList);

		// cpu gpu同期
		pCmdQueue->Signal(pFence, ++fenceValue);

		// gpu側の値とcpu側の値が一致するまでイベント作成して待つ
		const UInt64 gpuFenceValue = pFence->GetCompletedValue();
		if (gpuFenceValue != fenceValue)
		{
			HANDLE event = CreateEvent(NULL, false, false, NULL);
			pFence->SetEventOnCompletion(fenceValue,event);
			WaitForSingleObject(event, INFINITE);
		}

		pCmdAlloc->Reset();
		pCmdList->Reset(pCmdAlloc, nullptr);

		ppSwapchain->Present(0, 0);
		frameIndex = ppSwapchain->GetCurrentBackBufferIndex();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// コマンドリスト閉鎖
	pCmdList->Close();
	ID3D12CommandList* ppCmdList[] = { pCmdList };
	pCmdQueue->ExecuteCommandLists(_countof(ppCmdList), ppCmdList);

	// cpu gpu同期
	pCmdQueue->Signal(pFence, ++fenceValue);

	// gpu側の値とcpu側の値が一致するまでイベント作成して待つ
	const UInt64 gpuFenceValue = pFence->GetCompletedValue();
	if (gpuFenceValue != fenceValue)
	{
		HANDLE event = CreateEvent(NULL, false, false, NULL);
		pFence->SetEventOnCompletion(fenceValue, event);
		WaitForSingleObject(event, INFINITE);
	}

	pCmdAlloc->Reset();
	pCmdList->Reset(pCmdAlloc, nullptr);

	// 解放処理
	SAFE_RELEASE(pRenderTarget[0]);
	SAFE_RELEASE(pRenderTarget[1]);
	SAFE_RELEASE(pRtvHeap);

	SAFE_RELEASE(pSwapchain);
	SAFE_RELEASE(ppSwapchain);

	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);
	SAFE_RELEASE(pFactory6);
 
	SAFE_RELEASE(pCmdQueue);
	SAFE_RELEASE(pCmdAlloc);
	SAFE_RELEASE(pCmdList);

	SAFE_RELEASE(pFence);

	ID3D12DebugDevice* pDebugDevice = nullptr;
	pDevice->QueryInterface(GET_IID(&pDebugDevice));
	pDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
	SAFE_RELEASE(pDevice);

	return 0;
}