file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
        *.cpp
        *.hpp
)

add_library(engine STATIC ${ENGINE_SRC})

# Include directories
find_package(yaml-cpp CONFIG REQUIRED)

target_include_directories(engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/glfw/include
        ${CMAKE_SOURCE_DIR}/external/DirectX-Headers/include/directx
        ${CMAKE_SOURCE_DIR}/external/glm
        $<TARGET_PROPERTY:yaml-cpp::yaml-cpp,INTERFACE_INCLUDE_DIRECTORIES>
)

# Source files for glad
target_sources(engine PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

# === Required packages ===
find_package(assimp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

# === Link libraries ===
target_link_libraries(engine
        PRIVATE
        glfw
        glm
        assimp::assimp
        yaml-cpp::yaml-cpp
        d3d12
        dxgi
        dxguid
        d3dcompiler
)

# === DirectXTex manual import ===
set(DIRECTXTEX_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg/installed/x64-windows")

set(DIRECTXTEX_INCLUDE_DIRS "${DIRECTXTEX_ROOT}/include")
set(DIRECTXTEX_LIBRARIES "${DIRECTXTEX_ROOT}/lib/DirectXTex.lib")

target_include_directories(engine PRIVATE ${DIRECTXTEX_INCLUDE_DIRS})
target_link_libraries(engine PRIVATE ${DIRECTXTEX_LIBRARIES})


# Добавляем include и lib для DirectXTex
target_include_directories(engine PRIVATE ${DIRECTXTEX_INCLUDE_DIRS})
target_link_libraries(engine PRIVATE ${DIRECTXTEX_LIBRARIES})





# === Компиляция HLSL в CSO ===

# Папки с исходниками и результатами
set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/engine/render/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/assets/shaders)
set(SHADER_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders) # <-- для копирования рядом с exe

file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})
file(MAKE_DIRECTORY ${SHADER_RUNTIME_DIR})

# === Поиск FXC.exe из Windows SDK ===
find_program(FXC_EXECUTABLE fxc.exe
    HINTS
        "$ENV{WindowsSdkVerBinPath}/x64"
        "$ENV{WindowsSdkBinPath}/x64"
        "C:/Program Files (x86)/Windows Kits/10/bin/x64"
        "C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64"
        "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/x64"
)

if (NOT FXC_EXECUTABLE)
    message(FATAL_ERROR "❌ fxc.exe not found! Убедись, что установлен Windows 10 SDK (bin/x64/fxc.exe).")
else()
    message(STATUS "✅ FXC найден: ${FXC_EXECUTABLE}")
endif()

# === Vertex Shader ===
add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/VertexShader.cso
    COMMAND "${FXC_EXECUTABLE}"
        /T vs_5_0 /E main
        /Fo "${SHADER_OUTPUT_DIR}/VertexShader.cso"
        "${SHADER_SOURCE_DIR}/VertexShader.hlsl"
    DEPENDS "${SHADER_SOURCE_DIR}/VertexShader.hlsl"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
    COMMENT "Compiling VertexShader.hlsl → VertexShader.cso"
)

# === Pixel Shader ===
add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/PixelShader.cso
    COMMAND "${FXC_EXECUTABLE}"
        /T ps_5_0 /E main
        /Fo "${SHADER_OUTPUT_DIR}/PixelShader.cso"
        "${SHADER_SOURCE_DIR}/PixelShader.hlsl"
    DEPENDS "${SHADER_SOURCE_DIR}/PixelShader.hlsl"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
    COMMENT "Compiling PixelShader.hlsl → PixelShader.cso"
)

# === Цель сборки шейдеров ===
add_custom_target(Shaders ALL
    DEPENDS
        ${SHADER_OUTPUT_DIR}/VertexShader.cso
        ${SHADER_OUTPUT_DIR}/PixelShader.cso
)

# === Копирование .cso рядом с exe ===
add_custom_command(TARGET Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_RUNTIME_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SHADER_OUTPUT_DIR}/VertexShader.cso"
        "${SHADER_RUNTIME_DIR}/VertexShader.cso"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SHADER_OUTPUT_DIR}/PixelShader.cso"
        "${SHADER_RUNTIME_DIR}/PixelShader.cso"
    COMMENT "📦 Copying shaders to runtime directory: ${SHADER_RUNTIME_DIR}"
)
