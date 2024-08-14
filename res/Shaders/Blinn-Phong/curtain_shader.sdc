#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time; // ����һ��ʱ��������ڶ�̬Ч��

void main()
{
    vec3 pos = aPos;
    // ���Ӱڶ�Ч�������������� y ������һ����Χ��ʱ��Ч

    const float wind_strength = 10.0;     // ���ǿ��
    const float wind_frequency = 1.2;     // ���Ƶ��
    const float max_pos_y = 250;

    if (pos.z > 20.0 && pos.z < 30.0 && pos.y < max_pos_y) {

        float y_scale = (max_pos_y - pos.y) / max_pos_y; 

        pos.y += sin(time * wind_frequency + 0.02 * pos.x) * wind_strength * y_scale;
        pos.x += 0.1 * sin(time * wind_frequency + 0.02 * pos.x) * wind_strength * y_scale;
        pos.z += 2 * abs(sin(time * wind_frequency + 0.02 * pos.x)) * wind_strength * y_scale;
    }

    FragPos = vec3(model * vec4(pos, 1.0));
    FragPosLightSpace = lightSpaceMatrix*vec4(FragPos,1.0f);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}

#shader fragment
#version 330 core
#define NR_POINT_LIGHT  7
out vec4 FragColor;

in  vec3 FragPos;
in  vec3 Normal;
in  vec2 TexCoords;
in  vec4 FragPosLightSpace;

uniform    vec3 aAmbient;
uniform    vec3 aDiffuse;
uniform    vec3 aSpecular;
    


struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};





 


uniform vec3 viewPos;




uniform float shininess;
uniform DirLight dirLight;
uniform PointLight plight[NR_POINT_LIGHT];

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowmap;

uniform sampler2D texture_specular1;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float ShadowCalculation(PointLight light,vec4 fragPosLightSpace)
{
    float shadow=0;
    vec3 lightDir = normalize(light.position - FragPos);
    float bias =max(0.010 * (1.0 - dot(Normal, lightDir)), 0.001);
    vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
 
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowmap, projCoords.xy).r; 
 
    float currentDepth = projCoords.z;
    // PCF
    for(int i=-4;i<=4;++i)
    {
        for(int j=-4;j<=4;j++)
        {
            //  ������Χ����ShadowMap�е����
            float closestDepth = texture(shadowmap, projCoords.xy+vec2(i,j)*texelSize).r; 
            shadow += currentDepth-bias > closestDepth  ? 1.0 : 0.0;
        }
    }

    

    return shadow/81.0f;
}


void main()
{    
    float shadow = ShadowCalculation(plight[6],FragPosLightSpace);
    shadow = min(shadow,0.85f);
    
    vec4 texColor = texture(texture_diffuse1,TexCoords);
    if(texColor.a < 0.1)
        discard;
    
    

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i =0;i<NR_POINT_LIGHT;i++){
        result += CalcPointLight(plight[i], norm, FragPos, viewDir);    
    }
    FragColor = vec4((1.0f-shadow)*(result+vec3(texColor)),1.0f);
}


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient * (aAmbient);
    vec3 diffuse = light.diffuse * diff * (aDiffuse);
    vec3 specular = light.specular * spec * aSpecular;
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * (aAmbient);
    vec3 diffuse = light.diffuse * diff * (aDiffuse);
    vec3 specular = light.specular * spec * aSpecular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}