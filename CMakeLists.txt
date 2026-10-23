cmake_minimum_required(VERSION 3.20)
project(GLSolution CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Find system OpenGL
find_package(OpenGL REQUIRED)

# Force GCC to treat .ixx extension as standard C++ files
set_source_files_properties(
    OpenGL.SharedModule/src/abstraction/OpenGlShape.hpp
    OpenGL.SharedModule/src/abstraction/Drawer.hpp
    OpenGL.SharedModule/src/abstraction/SharedModule.hpp
    PROPERTIES LANGUAGE CXX
)

#-------------------------------------------------------------------------------
# OpenGL.SharedModule (Static Library)
#-------------------------------------------------------------------------------
add_library(OpenGL.SharedModule STATIC
    # OpenGL.SharedModule/src/ImGuiFileDialog.cpp
    OpenGL.SharedModule/src/glad.c
    OpenGL.SharedModule/src/stb_image.cpp
    OpenGL.SharedModule/src/imgui/imgui.cpp
    OpenGL.SharedModule/src/imgui/imgui_demo.cpp
    OpenGL.SharedModule/src/imgui/imgui_draw.cpp
    OpenGL.SharedModule/src/imgui/imgui_impl_glfw.cpp
    OpenGL.SharedModule/src/imgui/imgui_impl_opengl3.cpp
    OpenGL.SharedModule/src/imgui/imgui_stdlib.cpp
    OpenGL.SharedModule/src/imgui/imgui_tables.cpp
    OpenGL.SharedModule/src/imgui/imgui_widgets.cpp
    OpenGL.SharedModule/src/implementation/Drawer.cpp
    OpenGL.SharedModule/src/implementation/OpenGlShape.cpp
    OpenGL.SharedModule/src/implementation/SharedModule.cpp
    OpenGL.SharedModule/src/abstraction/OpenGlShape.hpp
    OpenGL.SharedModule/src/abstraction/Drawer.hpp
    OpenGL.SharedModule/src/abstraction/SharedModule.hpp
)

target_include_directories(OpenGL.SharedModule PUBLIC
    OpenGL.SharedModule/dependencies/include
    OpenGL.SharedModule/dependencies/include/imgui
    OpenGL.SharedModule/dependencies/include/imgui/backends
    # OpenGL.SharedModule/dependencies/include/imGuiFileDialog
    OpenGL.SharedModule/dependencies/include/glad
    OpenGL.SharedModule/dependencies/include/GLFW
)

target_link_libraries(OpenGL.SharedModule PUBLIC
    OpenGL::GL
)

# Automatically find all .cpp and .c files in the GLProject folder
file(GLOB GLPROJECT_SOURCES
    "GLProject/*.cpp"
    "GLProject/*.c"
    "GLProject/**/*.cpp"
    "GLProject/**/*.c"
)

# Pass the detected list and any extra shared source files to add_executable
add_executable(GLProject
    ${GLPROJECT_SOURCES}
    OpenGL.SharedModule/src/glad.c
)

link_directories(OpenGL.SharedModule/dependencies/libs)

target_include_directories(GLProject PRIVATE
    GLProject
    OpenGL.SharedModule/dependencies/include
    OpenGL.SharedModule/dependencies/include/GLFW
    OpenGL.SharedModule/dependencies/include/imgui
    OpenGL.SharedModule/dependencies/include/imgui/backends
    # OpenGL.SharedModule/dependencies/include/imGuiFileDialog
    OpenGL.SharedModule/dependencies/include/glad
)

if(WIN32)
target_link_libraries(GLProject PRIVATE
    OpenGL.SharedModule
    glfw3
    assimp
    spdlog
    OpenGL::GL
)
else()
target_link_libraries(GLProject PRIVATE
    OpenGL.SharedModule
    glfw
    assimp
    spdlog
    OpenGL::GL
)
endif()

add_custom_command(
    TARGET GLProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/GLProject
            $<TARGET_FILE_DIR:GLProject>
    COMMENT "Syncing GLProject directory contents and assets to build output..."
)
