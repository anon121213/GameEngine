#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "components/Transform.hpp"
#include "render/components/RenderMeshComponent.hpp"

class DX12Renderer {
public:
    bool Init(HWND hwnd, int width, int height);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void DrawMesh(RenderMeshComponent& mesh, const Transform& transform);
    void CreateMeshBuffers(RenderMeshComponent& mesh) const;

    void SetViewProjection(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
    float GetAspectRatio() const;
private:
    bool CreateDevice();
    bool CreateCommandObjects();
    bool CreateSwapChain(HWND hwnd, int width, int height);
    bool CreateRenderTargetViews();
    bool CreateRootSignature();
    bool CreatePipelineState();
    bool CreateConstantBuffer();

    static constexpr int FrameCount = 2;

    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FrameCount];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

    UINT rtvDescriptorSize = 0;
    UINT frameIndex = 0;

    int width = 0;
    int height = 0;

    struct alignas(256) MVP {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
    D3D12_GPU_VIRTUAL_ADDRESS cbAddress;
    MVP mvpData = {};
};