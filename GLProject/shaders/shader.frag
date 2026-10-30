#version 330 core
// Receive from the vertex shader
in vec3 color;

// Uniform time sent from the CPU
uniform float time;

// Output to screen
out vec4 FragColor;

//Note that we could have defined uniforms here and used them as well

void main()
{
    FragColor = vec4(color.x, color.y, color.z, 1.0);
}
