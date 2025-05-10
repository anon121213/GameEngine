cmake_minimum_required(VERSION 3.20)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

add_subdirectory(app)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/DirectX-Headers)
add_subdirectory(engine)
