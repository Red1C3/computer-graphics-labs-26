#version 330 core
// Stored in the VBO
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Uniform matrices sent from the CPU
uniform mat4 model,view,perspective;

// Send to the fragment shader
out vec2 uvOut;
out vec3 normalOut;
out vec3 posOut;

void main(){
    // Passed as is to the fragment shader
    uvOut=uv;

    // Notice how the matrices are applied in order
    gl_Position=perspective*view*model*vec4(pos.x,pos.y,pos.z,1.0);
    
    posOut=vec3(model*vec4(pos,1)); // Vertex location in world space
    normalOut=normalize(vec3(transpose(inverse(model))*vec4(normal,0))); //Fragment in world space
}