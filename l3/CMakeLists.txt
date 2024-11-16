cmake_minimum_required(VERSION 3.11)

project(l3 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# lab files
file(GLOB SRC_FILES src/*.cpp)
file(GLOB HEADER_FILES lib/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
target_include_directories(${PROJECT_NAME} PUBLIC lib src)

# include libraries
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ../vendor/glad/include)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm-header-only glad)