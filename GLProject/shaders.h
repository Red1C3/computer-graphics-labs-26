#pragma once

const char* VERTEX_SHADER = R"(
    #version 330 core
    layout(location = 0) in vec3 pos; // A single point position
    layout(location = 1) in vec3 vertexColor; // A single point color

    out vec3 color; // Pass the color "out" to the fragment shader
    void main(){
        color=vertexColor;
        gl_Position=vec4(pos.x,pos.y,pos.z,1.0); // Set the final value of the position on screen
    }
)";


const char* FRAGMENT_SHADER = R"(
    #version 330 core
    // Take "in" the color value from vertex shader
    // Note that the names "color" must be identical in both vertex and fragment shaders
    in vec3 color;

    // The final color that is drawn on screen for a single pixel (or fragment)
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(color.x, color.y, color.z, 1.0f); // Interpolate the vertices color from all of the vertices color
    }
)";
