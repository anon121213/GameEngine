#include "DX12Renderer.hpp"
#include <d3dx12.h>
#include <render/dx12/managers/Mesh/MeshManager.hpp>

#include "core/Log.hpp"
#include "services/ServiceLocator.hpp"

using namespace DirectX;

bool DX12Renderer::Init(const HWND hwnd, const int width, const int height) {
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

void DX12Renderer::DrawMesh(RenderMeshComponent& mesh, const Transform& transform) const {
    const auto uploader    = ServiceLocator::Get<RendererMeshUploadService>();
    const auto meshManager = ServiceLocator::Get<MeshManager>();

    if (mesh.subMeshes.empty()) {
        LOG_WARN("[DX12Renderer] entity has NO submeshes, skip draw.");
        return;
    }

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

    int drawn = 0;
    for (size_t i = 0; i < mesh.subMeshes.size(); ++i)
    {
        auto& sub = mesh.subMeshes[i];
        const MeshAsset& asset = meshManager->LoadMesh(sub.meshPath);

        if (asset.vertices.empty()) {
            LOG_ERROR("[DX12Renderer] submesh[{}] key='{}' has EMPTY CPU data (verts=0). Skip.",
                      i, sub.meshPath);
            continue;
        }

        uploader->UploadIfNeeded(commandService.GetDevice(), sub); // <- передаем asset, чтобы аплоадер знал размеры

        if (!sub.vertexBuffer || sub.vertexBufferView.SizeInBytes == 0 || sub.vertexBufferView.StrideInBytes == 0) {
            LOG_ERROR("  INVALID VB: resource={}, size={}, stride={}",
                      (void*)sub.vertexBuffer.Get(), sub.vertexBufferView.SizeInBytes, sub.vertexBufferView.StrideInBytes);
            continue;
        }

        if (!asset.indices.empty()) {
            if (!sub.indexBuffer || sub.indexBufferView.SizeInBytes == 0) {
                LOG_ERROR("  INVALID IB: resource={}, size={}",
                          (void*)sub.indexBuffer.Get(), sub.indexBufferView.SizeInBytes);
                continue;
            }
        } else {
            LOG_WARN("  Non-indexed draw path for key='{}'", sub.meshPath);
        }

        commandService.DrawMesh(sub, asset);
        ++drawn;
    }
}

void DX12Renderer::SetViewProjection(const XMMATRIX& view, const XMMATRIX& proj) {
    viewMatrix = view;
    projMatrix = proj;
}

float DX12Renderer::GetAspectRatio() const {
    return static_cast<float>(width) / height;
}
