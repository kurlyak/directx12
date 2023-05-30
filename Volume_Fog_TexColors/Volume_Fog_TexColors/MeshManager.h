//======================================================================================
//	Ed Kurlyak 2023 Volume Fog DirectX12
//======================================================================================

#ifndef _MESHMANAGER_
#define _MESHMANAGER_

#include <windows.h>
#include <windowsx.h>

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <D3Dcompiler.h>
#include <string>
#include <comdef.h>
#include <memory>
#include <DirectXMath.h>
#include <vector>
#include <DirectXColors.h>
#include <array>
#include <unordered_map>
#include <DirectXCollision.h>

#include <directxmath.h>

#include "d3dUtil.h"

#include "Timer.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

template<typename T>
class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
		mIsConstantBuffer(isConstantBuffer)
	{
		mElementByteSize = sizeof(T);

		if (isConstantBuffer)
			mElementByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(T));

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer)));

		ThrowIfFailed(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
	}

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
	~UploadBuffer()
	{
		if (mUploadBuffer != nullptr)
			mUploadBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource* Resource()const
	{
		return mUploadBuffer.Get();
	}

	void CopyData(int elementIndex, const T& data)
	{
		memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE* mMappedData = nullptr;

	UINT mElementByteSize = 0;
	bool mIsConstantBuffer = false;
};

static DirectX::XMFLOAT4X4 Identity4x4()
{
	static DirectX::XMFLOAT4X4 I(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return I;
}


struct ObjectConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj = Identity4x4();
	float ZFar = 0.0f;
};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
};

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;
};


struct MeshGeometry
{
	std::string Name;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
	}
};

enum { A, B, C, D, E, F, G, H };

class CMeshManager
{
public:
	CMeshManager();
	~CMeshManager();

	void Init_MeshManager(HWND hWnd);
	void Update_MeshManager();
	void Draw_MeshManager();

private:
	void EnableDebugLayer_CreateFactory();
	void Create_Device();
	void CreateFence_GetDescriptorsSize();
	void Check_Multisample_Quality();
	void Create_CommandList_Allocator_Queue();
	void Create_SwapChain();
	void Resize_SwapChainBuffers();
	void FlushCommandQueue();
	void Create_Dsv_DescriptorHeaps_And_View();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView();
	void Execute_Init_Commands();
	void Update_ViewPort_And_Scissor();
	void Create_Constant_Buffer_Pass1_Pass2();
	void Create_Main_RenderTargetHeap_And_View_Pass3();
	void Create_RootSignature();
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	void Create_Cube_Shaders_And_InputLayout_Pass1_Pass2();
	void Create_Cube_Geometry_Pass1_Pass2();
	void Create_PipelineStateObject_Pass1();
	void Create_PipelineStateObject_Pass2();
	void Create_RTVDescriptorHeap_Pass1_Pass2();
	void Create_RTView_Pass1_Pass2();
	void Create_SRDescriptorHead_And_View_For_Pass3();
	void Create_ScreenAlighedQuad_Shaders_And_InputLayout_Pass3();
	void Create_ScreenAlighedQuad_Geometry_Pass3();
	void Create_PipelineStateObject_Pass3();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
	ID3D12Resource* CurrentBackBuffer();

	CTimer m_Timer;

	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;

	UINT m_RtvDescriptorSize = 0;
	UINT m_DsvDescriptorSize = 0;
	UINT m_CbvSrvUavDescriptorSize = 0;

	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	bool      m_4xMsaaState = false;
	UINT      m_4xMsaaQuality = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_DirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	int m_ClientWidth = 800;
	int m_ClientHeight = 600;

	static const int m_SwapChainBufferCount = 2;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[m_SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

	HWND m_hWnd;

	UINT64 m_CurrentFence = 0;

	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvHeapPass3;

	D3D12_VIEWPORT m_ScreenViewport;
	D3D12_RECT mScissorRect;

	std::unique_ptr<UploadBuffer<ObjectConstants>> m_ObjectCB = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeapPass3;

	int m_CurrBackBuffer = 0;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> m_VsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_PsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

	std::unique_ptr<MeshGeometry> m_Cube = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSOPass1 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSOPass2 = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeapRTTex;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_RTVTexHandlePass1;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_RTVTexHandlePass2;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargetTexPass1;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargetTexPass2;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SrvDescriptorHeapSAQ = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> m_VsByteCodeSAQ = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_PsByteCodeSAQ = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayoutSAQ;

	std::unique_ptr<MeshGeometry> m_SQABuff = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSOSAQ = nullptr;

	DirectX::XMFLOAT4X4 m_World = Identity4x4();
	DirectX::XMFLOAT4X4 m_View = Identity4x4();
	DirectX::XMFLOAT4X4 m_Proj = Identity4x4();

	float m_ZFar = 100.0f;
};

#endif