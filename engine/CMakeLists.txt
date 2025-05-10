file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS *.cpp *.hpp)

add_library(engine STATIC ${ENGINE_SRC})

target_include_directories(engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/glfw/include
        ${CMAKE_SOURCE_DIR}/external/DirectX-Headers/include/directx
        ${CMAKE_SOURCE_DIR}/external/glm
)

target_sources(engine PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_link_libraries(engine
        PRIVATE
        glfw
        glm        
)

target_link_libraries(engine
        PRIVATE
        d3d12
        dxgi
        d3dcompiler
)
