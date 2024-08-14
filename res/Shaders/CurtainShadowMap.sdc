#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform float time; 

void main()
{
    vec3 pos = aPos;

    const float wind_strength = 10.0;    
    const float wind_frequency = 1.2;   
    const float max_pos_y = 250;

    if (pos.z > 20.0 && pos.z < 30.0 && pos.y < max_pos_y) {

        float y_scale = (max_pos_y - pos.y) / max_pos_y; 

        pos.y += sin(time * wind_frequency + 0.02 * pos.x) * wind_strength * y_scale;
        pos.x += 0.1 * sin(time * wind_frequency + 0.02 * pos.x) * wind_strength * y_scale;
        pos.z += 2 * abs(sin(time * wind_frequency + 0.02 * pos.x)) * wind_strength * y_scale;
    }

    

    
    gl_Position = lightSpaceMatrix * model * vec4(pos, 1.0);
}

#shader fragment
#version 330 core

void main()
{    
 

}
