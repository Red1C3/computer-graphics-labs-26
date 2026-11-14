#version 330 core
// Receive from the vertex shader
in vec2 uvOut;

// Output to screen
out vec4 FragColor;

// The current texture
uniform sampler2D tex;

void main()
{
    // We take a pixel (sample) from the image into the model
    FragColor = texture(tex, uvOut);
}
