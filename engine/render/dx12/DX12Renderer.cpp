#include "DX12Renderer.hpp"
#include <d3dx12.h>

using namespace DirectX;

bool DX12Renderer::Init(HWND hwnd, int width, int height) {
    this->width = width;
    this->height = height;

    if (!commandService.CreateDevice()) return false;
    if (!commandService.CreateCommandObjects()) return false;
    if (!swapChainService.Create(commandService.GetCommandQueue(), hwnd, width, height)) return false;
    if (!rtvService.Create(commandService.GetDevice(), swapChainService.GetSwapChain())) return false;
    if (!depthService.Create(commandService.GetDevice(), width, height)) return false;
    if (!rootSigService.Create(commandService.GetDevice())) return false;
    if (!constantBufferService.Create(commandService.GetDevice())) return false;
    if (!pipelineService.Create(commandService.GetDevice(), rootSigService.Get())) return false;

    swapChainService.UpdateFrameIndex();
    rtvService.UpdateFrameIndex(swapChainService.GetCurrentFrameIndex());

    return true;
}

void DX12Renderer::Shutdown() {}

void DX12Renderer::BeginFrame() const {
    commandService.BeginFrame(pipelineService.GetPipelineState());

    commandService.PrepareRenderTarget(
        rtvService.GetRTVHandle(),
        rtvService.GetDescriptorSize(),
        rtvService.GetCurrentRenderTarget(),
        width, height,
        depthService.GetDSVHandle());
}

void DX12Renderer::EndFrame() {
    commandService.EndFrame(rtvService.GetCurrentRenderTarget());
    swapChainService.Present();
    swapChainService.UpdateFrameIndex();
    rtvService.UpdateFrameIndex(swapChainService.GetCurrentFrameIndex());
}

void DX12Renderer::DrawMesh(RenderMeshComponent& mesh, const Transform& transform) {
    meshUploader.UploadIfNeeded(commandService.GetDevice(), mesh);

    constantBufferService.UpdateModelMatrix(
        mesh.constantBufferIndex,
        transform.GetMatrixDX(),
        viewMatrix,
        projMatrix
    );

    commandService.SetGraphicsState(
        rootSigService.Get(),
        constantBufferService.GetGPUAddress(mesh.constantBufferIndex),
        pipelineService.GetPipelineState()
    );

    commandService.DrawMesh(mesh);
}

void DX12Renderer::SetViewProjection(const XMMATRIX& view, const XMMATRIX& proj) {
    viewMatrix = view;
    projMatrix = proj;
}

float DX12Renderer::GetAspectRatio() const {
    return static_cast<float>(width) / height;
}
