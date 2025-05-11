#pragma once
#include <DirectXMath.h>
#include "services/RendererCommandService.hpp"
#include "services/RendererSwapChainService.hpp"
#include "services/RendererRTVService.hpp"
#include "services/RendererPipelineService.hpp"
#include "services/RendererRootSignatureService.hpp"
#include "services/RendererConstantBufferService.hpp"
#include "services/RendererDepthStencilService.hpp"
#include "services/RendererMeshUploadService.hpp"
#include "components/Transform.hpp"

class DX12Renderer {
public:
    bool Init(HWND hwnd, int width, int height);
    void Shutdown();

    void BeginFrame() const;
    void EndFrame();

    void DrawMesh(RenderMeshComponent& mesh, const Transform& transform);
    void SetViewProjection(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
    float GetAspectRatio() const;

private:
    int width = 0;
    int height = 0;
    DirectX::XMMATRIX viewMatrix{};
    DirectX::XMMATRIX projMatrix{};

    RendererCommandService commandService;
    RendererSwapChainService swapChainService;
    RendererRTVService rtvService;
    RendererRootSignatureService rootSigService;
    RendererPipelineService pipelineService;
    RendererConstantBufferService constantBufferService;
    RendererMeshUploadService meshUploader;
    RendererDepthStencilService depthService;
};
