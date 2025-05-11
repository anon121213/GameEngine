file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
        *.cpp
        *.hpp
)

add_library(engine STATIC ${ENGINE_SRC})

# Include directories
target_include_directories(engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/glfw/include
        ${CMAKE_SOURCE_DIR}/external/DirectX-Headers/include/directx
        ${CMAKE_SOURCE_DIR}/external/glm
)

# Source files for glad
target_sources(engine PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Required packages
find_package(assimp CONFIG REQUIRED)

# Link libraries
target_link_libraries(engine
        PRIVATE
        glfw
        glm
        assimp::assimp
        d3d12
        dxgi
        d3dcompiler
)
