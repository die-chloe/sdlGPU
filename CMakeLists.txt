cmake_minimum_required(VERSION 3.15)



project(sdlGPU)
set(CMAKE_CXX_STANDARD 23)


#dependencies
set(LIBS)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/glm)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})
#set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3 glm::glm)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/SDL/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
)

if(WIN32)

    # Copy SDL3.dll to the output directory on Windows
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()



# SHANDER COMPILATION

# Find Shader Source Files
file(GLOB_RECURSE VERTEX_SHADER_SOURCES shaders/*.vert.hlsl)
file(GLOB_RECURSE FRAGMENT_SHADER_SOURCES shaders/*.frag.hlsl)

# Create a directory for compiled shaders
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_SOURCE_DIR}/shaders/compiled
)

find_program(glslc NAMES glslc)

# Compile Vertex Shaders
foreach(SHADER_FILE ${VERTEX_SHADER_SOURCES})
    get_filename_component(SHADER_SRC ${SHADER_FILE} NAME_WE)
    message(STATUS " ${glslc} -o ${CMAKE_SOURCE_DIR}/shaders/compiled/${SHADER_SRC}.vert.spv -x hlsl -fshader-stage=vertex ${SHADER_FILE}")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${glslc}
        ARGS -o ${CMAKE_SOURCE_DIR}/shaders/compiled/${SHADER_SRC}.vert.spv -x hlsl -fshader-stage=vertex ${SHADER_FILE}
    )

endforeach(SHADER_FILE ${VERTEX_SHADER_SOURCES})

# Compile Fragment Shaders
foreach(SHADER_FILE ${FRAGMENT_SHADER_SOURCES})
    get_filename_component(SHADER_SRC ${SHADER_FILE} NAME_WE)
    message(STATUS " ${glslc} -o ${CMAKE_SOURCE_DIR}/shaders/compiled/${SHADER_SRC}.frag.spv -x hlsl -fshader-stage=fragment ${SHADER_FILE}")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${glslc}
        ARGS -o ${CMAKE_SOURCE_DIR}/shaders/compiled/${SHADER_SRC}.frag.spv -x hlsl -fshader-stage=fragment ${SHADER_FILE}
    )
endforeach(SHADER_FILE ${FRAGMENT_SHADER_SOURCES})

# Copy compiled shaders to the output directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders/compiled
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)