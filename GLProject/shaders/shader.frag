#version 330 core
// Receive from the vertex shader
in vec2 uvOut;
in vec3 normalOut;
in vec3 posOut;

// Output to screen
out vec4 FragColor;

const float ambientFactor=0.1;

uniform sampler2D tex;

// First light properties
uniform vec4 light1_ambient=vec4(1),light1_diffuse=vec4(1),light1_specular=vec4(1);
uniform vec3 light1_direction=vec3(-1);

// Second light properties
uniform vec4 light2_ambient=vec4(1),light2_diffuse=vec4(1),light2_specular=vec4(1);
uniform vec3 light2_position=vec3(-2,-2,-7);

// Material properties
uniform vec4 material_ambient=vec4(1),material_diffuse=vec4(1),material_specular=vec4(1);
uniform float shininess=256;

uniform vec3 cameraPos=vec3(0);

// Transparent control variable
uniform bool transparent=false;

void main()
{
    vec3 normal=normalOut;
    vec3 pos=posOut;
    
    vec4 texColor=texture(tex,uvOut);
    
    FragColor=vec4(0);
    
    // Light 1 (Directional Light)
    vec3 fragToLight=-light1_direction;
    float diffuseFactor=max(dot(fragToLight,normal),0);
    vec3 fragToObserver=normalize(-pos+cameraPos);
    vec3 halfWay=normalize(fragToLight+fragToObserver);
    float specFactor=max(pow(dot(halfWay,normal),shininess),0);
    
    
    FragColor+=diffuseFactor*light1_diffuse*material_diffuse*texColor + specFactor*light1_specular*material_specular+ambientFactor*light1_ambient*material_ambient;
    
    // Light 2 (Point Light)
    fragToLight=normalize(-pos+light2_position); // Only difference is the fragment to light vector
    diffuseFactor=max(dot(fragToLight,normal),0);
    halfWay=normalize(fragToLight+fragToObserver);
    specFactor=max(pow(dot(halfWay,normal),shininess),0);
    
    FragColor+=diffuseFactor*light2_diffuse*material_diffuse*texColor + specFactor*light2_specular*material_specular+ambientFactor*light2_ambient*material_ambient;
    if(transparent){
      FragColor.a=0.7;
    }else{
      FragColor.a=1;
    }
} 