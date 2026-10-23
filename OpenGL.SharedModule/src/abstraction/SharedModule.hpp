// module; // optional. Defines the beginning of the global module fragment

// #include directives go here but only apply to this file.
// import statements aren't allowed here. They go in the module preamble, below.
#pragma once

#include <stdexcept>
#include <GLFW/glfw3.h>
#include <vector>

// export module SharedModule;

class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Function not yet implemented") { };
};

void setup_ImGUI(GLFWwindow* window);
void render_ImGui();
void shutdown_ImGui();

std::vector<float> read_float_array_from_file(const char* filename);
unsigned int load_texture(const char* filename);