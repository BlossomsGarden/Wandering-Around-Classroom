#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1 ) in vec3  aNormal;
layout(location = 2) in vec2  aTexCoord;

out vec2 v_TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    v_TexCoord = aTexCoord;
}






#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
    vec4 TexColor = texture(u_Texture, v_TexCoord);
    color =TexColor;
};
