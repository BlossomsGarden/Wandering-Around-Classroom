#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aInstanceModel;


uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{   

   

   
    gl_Position = lightSpaceMatrix *aInstanceModel* model * vec4(aPos, 1.0f);
}



#shader fragment
#version 330 core
void main()
{

}