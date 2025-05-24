cmake_minimum_required(VERSION 3.15)



project(sdlGPU)
set(CMAKE_CXX_STANDARD 23)


#find_package(SDL3 REQUIRED)
add_executable(sdlGPU src/main.cpp)
#target_link_libraries(sdlGPU PRIVATE SDL3::SDL3)
