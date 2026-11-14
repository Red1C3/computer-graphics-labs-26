#version 330 core
// Stored in the VBO
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

// Uniform matrices sent from the CPU
uniform mat4 model, view, perspective;

// Send to the fragment shader
out vec2 uvOut;

void main() {
    // Passed as is to the fragment shader
    uvOut = uv;

    // Notice how the matrices are applied in order
    gl_Position = perspective * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
}
