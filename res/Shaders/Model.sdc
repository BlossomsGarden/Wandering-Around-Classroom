#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform Mat{
    vec3 aAmbient;
    vec3 aDiffuse;
    vec3 aSpecular;

};


out vec2 TexCoords;

out vec3 Ambient;
out vec3 Diffuse;
out vec3 Specular;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}



#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


in vec3 Ambient;
in vec3 Diffuse;
in vec3 Specular;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;


uniform sampler2D texture_specular1;

void main()
{    
    vec3 diff1 = vec3(texture(texture_diffuse1, TexCoords));
    vec3 diff2 = vec3(texture(texture_diffuse2, TexCoords));

    vec3 diff = mix(diff1,diff2,0.5);  
    vec3 specular =vec3( texture(texture_specular1,TexCoords));

    FragColor = vec4(diff+specular,1.0f);
}