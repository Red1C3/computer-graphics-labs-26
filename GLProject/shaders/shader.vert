#version 330 core
// Stored in the VBO
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertexColor;

// Uniform matrices sent from the CPU
uniform mat4 model, view, perspective;

// Uniform time sent from the CPU
uniform float time;

// Send to the fragment shader
out vec3 color;

void main() {
    // Passed as is to the fragment shader
    color = vertexColor;

    // Notice how the matrices are applied in order
    gl_Position = perspective * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
}
