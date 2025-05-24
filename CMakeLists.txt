cmake_minimum_required(VERSION 3.15)



project(sdlGPU)
set(CMAKE_CXX_STANDARD 23)


#dependencies
set(LIBS)

add_subdirectory(vendor/SDL)


add_executable(${PROJECT_NAME} src/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

if(WIN32)

    # Copy SDL3.dll to the output directory on Windows
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()