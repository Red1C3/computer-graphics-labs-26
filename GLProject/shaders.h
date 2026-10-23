#pragma once

const char* VERTEX_SHADER = R"(
    #version 330 core
    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec3 vertexColor;

    out vec3 color;
    void main(){
        color=vertexColor;
        gl_Position=vec4(pos.x,pos.y,pos.z,1.0);
    }
)";


const char* FRAGMENT_SHADER = R"(
    #version 330 core
    in vec3 color;
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(color.x, color.y, color.z, 1.0f);
    }
)";
