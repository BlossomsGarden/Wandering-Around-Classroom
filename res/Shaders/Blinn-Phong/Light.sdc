#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 mColor;
uniform vec3 gColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection*view* model * vec4(aPos, 1.0f);

    mColor =gColor;
}

#shader fragment
#version 330 core  
layout(location = 0) out vec4 color;    



in vec3 mColor;

void main()  
{  

   color = vec4( mColor, 1.0f);
}  ;
