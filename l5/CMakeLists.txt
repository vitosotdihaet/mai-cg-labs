cmake_minimum_required(VERSION 3.11)

project(l5 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC_FILES src/*.cpp)
file(GLOB HEADER_FILES lib/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
target_include_directories(${PROJECT_NAME} PUBLIC lib src)

# include libraries
find_package(glm CONFIG REQUIRED)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only)