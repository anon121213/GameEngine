file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS *.cpp *.hpp)

add_library(engine STATIC ${ENGINE_SRC})

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(engine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_sources(engine PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_link_libraries(engine glfw)
