cmake_minimum_required(VERSION 3.11)

project(cg-labs)

if (OPT)
    add_compile_options(-O3)
endif ()


# setup common libs

# glfw -- https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source
add_subdirectory(vendor/glfw vendor/glfw/build)
# glm -- https://github.com/g-truc/glm?tab=readme-ov-file#build-and-install
add_subdirectory(vendor/glm vendor/glm/build)
# glad -- https://glad.dav1d.de/
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)
# game engine
add_subdirectory(engine engine/build)


add_subdirectory(l2)
add_subdirectory(l3)
add_subdirectory(l6)
