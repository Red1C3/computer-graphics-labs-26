#version 460 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    // Use the provided ourColor value
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}