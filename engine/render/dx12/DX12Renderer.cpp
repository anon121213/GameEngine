#include "DX12Renderer.hpp"
#include <d3dx12.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

bool DX12Renderer::Init(HWND hwnd, int width, int height) {
  DX12Renderer::width = width;
  DX12Renderer::height = height;
  
  if (!CreateDevice()) return false;
  if (!CreateCommandObjects()) return false;
  if (!CreateSwapChain(hwnd, width, height)) return false;
  if (!CreateRenderTargetViews()) return false;
  if (!CreateRootSignature()) return false;
  if (!CreateConstantBuffer()) return false;
  if (!CreatePipelineState()) return false;
  return true;
}

void DX12Renderer::Shutdown() {
  // Resources are cleaned up via ComPtr destructors
}

bool DX12Renderer::CreateDevice() {
  if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)))) {
    return false;
  }
  return true;
}

bool DX12Renderer::CreateCommandObjects() {
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) return false;

  if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) return
      false;

  if (FAILED(
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&
      commandList)))) return false;

  return true;
}

bool DX12Renderer::CreateSwapChain(HWND hwnd, int width, int height) {
  ComPtr<IDXGIFactory4> factory;
  if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) return false;

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = FrameCount;
  swapChainDesc.Width = width;
  swapChainDesc.Height = height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  ComPtr<IDXGISwapChain1> tempSwapChain;
  if (FAILED(factory->CreateSwapChainForHwnd(
    commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &tempSwapChain)))
    return false;

  if (FAILED(tempSwapChain.As(&swapChain))) return false;
  frameIndex = swapChain->GetCurrentBackBufferIndex();

  return true;
}

bool DX12Renderer::CreateRenderTargetViews() {
  D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
  heapDesc.NumDescriptors = FrameCount;
  heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

  if (FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap)))) return false;

  rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

  for (UINT i = 0; i < FrameCount; i++) {
    if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])))) return false;
    device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
    rtvHandle.Offset(1, rtvDescriptorSize);
  }

  return true;
}

void DX12Renderer::BeginFrame() {
  commandAllocator->Reset();
  commandList->Reset(commandAllocator.Get(), pipelineState.Get());

  mvpData.model = DirectX::XMMatrixIdentity();
  
  void* cbPtr = nullptr;
  D3D12_RANGE readRange{ 0, 0 };
  constantBuffer->Map(0, &readRange, &cbPtr);
  memcpy(cbPtr, &mvpData, sizeof(MVP));
  constantBuffer->Unmap(0, nullptr);

  D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f };
  D3D12_RECT scissorRect = { 0, 0, width, height };
  commandList->RSSetViewports(1, &viewport);
  commandList->RSSetScissorRects(1, &scissorRect);

  auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      renderTargets[frameIndex].Get(),
      D3D12_RESOURCE_STATE_PRESENT,
      D3D12_RESOURCE_STATE_RENDER_TARGET);
  commandList->ResourceBarrier(1, &barrier);

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);
  commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

  const float clearColor[] = { 0.2f, 0.3f, 0.4f, 1.0f };
  commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
  commandList->SetGraphicsRootSignature(rootSignature.Get());
  commandList->SetGraphicsRootConstantBufferView(0, cbAddress);
  commandList->SetPipelineState(pipelineState.Get());
}


void DX12Renderer::EndFrame() {
  CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
    renderTargets[frameIndex].Get(),
    D3D12_RESOURCE_STATE_RENDER_TARGET,
    D3D12_RESOURCE_STATE_PRESENT);
  commandList->ResourceBarrier(1, &barrier);

  commandList->Close();
  ID3D12CommandList *cmdsLists[] = {commandList.Get()};
  commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
  swapChain->Present(1, 0);

  frameIndex = swapChain->GetCurrentBackBufferIndex();
  commandAllocator->Reset();
  commandList->Reset(commandAllocator.Get(), nullptr);
}

void DX12Renderer::DrawMesh(RenderMeshComponent& mesh, const Transform& transform) const {
  CreateMeshBuffers(mesh);

  UpdateConstantBuffer(transform.GetMatrixDX());

  commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  commandList->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);

  if (!mesh.indices.empty()) {
    commandList->IASetIndexBuffer(&mesh.indexBufferView);
    commandList->DrawIndexedInstanced(static_cast<UINT>(mesh.indices.size()), 1, 0, 0, 0);
  } else {
    commandList->DrawInstanced(static_cast<UINT>(mesh.vertices.size()), 1, 0, 0);
  }
}

void DX12Renderer::CreateMeshBuffers(RenderMeshComponent& mesh) const {
  if (mesh.initialized) return;

  CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
  CD3DX12_RANGE readRange(0, 0);

  // Vertex Buffer
  UINT vertexBufferSize = static_cast<UINT>(mesh.vertices.size() * sizeof(Vertex));
  auto vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

  device->CreateCommittedResource(
      &heapProps,
      D3D12_HEAP_FLAG_NONE,
      &vbDesc,
      D3D12_RESOURCE_STATE_GENERIC_READ,
      nullptr,
      IID_PPV_ARGS(&mesh.vertexBuffer)
  );

  void* vbData = nullptr;
  mesh.vertexBuffer->Map(0, &readRange, &vbData);
  memcpy(vbData, mesh.vertices.data(), vertexBufferSize);
  mesh.vertexBuffer->Unmap(0, nullptr);

  mesh.vertexBufferView.BufferLocation = mesh.vertexBuffer->GetGPUVirtualAddress();
  mesh.vertexBufferView.StrideInBytes = sizeof(Vertex);
  mesh.vertexBufferView.SizeInBytes = vertexBufferSize;

  // Index Buffer
  if (!mesh.indices.empty()) {
    UINT indexBufferSize = static_cast<UINT>(mesh.indices.size() * sizeof(uint32_t));
    auto ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &ibDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mesh.indexBuffer)
    );

    void* ibData = nullptr;
    mesh.indexBuffer->Map(0, &readRange, &ibData);
    memcpy(ibData, mesh.indices.data(), indexBufferSize);
    mesh.indexBuffer->Unmap(0, nullptr);

    mesh.indexBufferView.BufferLocation = mesh.indexBuffer->GetGPUVirtualAddress();
    mesh.indexBufferView.SizeInBytes = indexBufferSize;
    mesh.indexBufferView.Format = DXGI_FORMAT_R32_UINT;
  }

  mesh.initialized = true;
}

bool DX12Renderer::CreateRootSignature() {
  CD3DX12_ROOT_PARAMETER rootParam;
  rootParam.InitAsConstantBufferView(0); 

  CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
  rootSigDesc.Init(1, &rootParam, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

  ComPtr<ID3DBlob> serializedRootSig = nullptr;
  ComPtr<ID3DBlob> errorBlob = nullptr;

  HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
                                           &serializedRootSig, &errorBlob);

  if (FAILED(hr)) {
    if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    return false;
  }

  if (FAILED(device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
                                         serializedRootSig->GetBufferSize(),
                                         IID_PPV_ARGS(&rootSignature)))) {
    return false;
                                         }

  return true;
}


bool DX12Renderer::CreatePipelineState() {
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> psBlob;

    // Загрузка .cso файлов
    if (FAILED(D3DReadFileToBlob(L"shaders/VertexShader.cso", &vsBlob)) ||
        FAILED(D3DReadFileToBlob(L"shaders/PixelShader.cso", &psBlob))) {
        MessageBoxA(nullptr, "Failed to load shader .cso files", "DX12", MB_OK | MB_ICONERROR);
        return false;
    }

    if (!rootSignature) {
        MessageBoxA(nullptr, "Root signature is null!", "DX12", MB_OK | MB_ICONERROR);
        return false;
    }

    // Вершинный layout под Vertex { XMFLOAT3 pos, XMFLOAT3 color }
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
    psoDesc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC{}; // отключим пока
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
    if (FAILED(hr)) {
        MessageBoxA(nullptr, "Failed to create graphics pipeline state", "DX12", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

bool DX12Renderer::CreateConstantBuffer() {
  CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
  auto cbDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(MVP) + 255) & ~255); // 256 byte aligned

  if (FAILED(device->CreateCommittedResource(
      &heapProps,
      D3D12_HEAP_FLAG_NONE,
      &cbDesc,
      D3D12_RESOURCE_STATE_GENERIC_READ,
      nullptr,
      IID_PPV_ARGS(&constantBuffer)
  ))) return false;

  cbAddress = constantBuffer->GetGPUVirtualAddress();
  return true;
}

void DX12Renderer::UpdateConstantBuffer(const DirectX::XMMATRIX& model) const {
  MVP mvp{};
  mvp.model = model;
  mvp.view = mvpData.view;
  mvp.projection = mvpData.projection;

  void* cbPtr = nullptr;
  D3D12_RANGE readRange{0, 0};
  if (SUCCEEDED(constantBuffer->Map(0, &readRange, &cbPtr))) {
    memcpy(cbPtr, &mvp, sizeof(MVP));
    constantBuffer->Unmap(0, nullptr);
  }
}


void DX12Renderer::SetViewProjection(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) {
  mvpData.view = view;
  mvpData.projection = proj;
}
float DX12Renderer::GetAspectRatio() const {
  return static_cast<float>(width) / height;
}
