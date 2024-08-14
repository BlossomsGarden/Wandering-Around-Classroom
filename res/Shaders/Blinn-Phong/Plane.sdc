#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;



out  vec3 FragPos;
out  vec3 Normal;
out  vec2 TexCoords;
out  vec4 FragPosLightSpace;

uniform mat4 lightSpaceMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{   
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragPosLightSpace = lightSpaceMatrix*vec4(FragPos,1.0f);
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * model *vec4(aPos, 1.0);
   
}



#shader fragment
#version 330 core
out vec4 FragColor;

in  vec3 FragPos;
in  vec3 Normal;
in  vec2 TexCoords;
in  vec4 FragPosLightSpace;

#define NR_POINT_LIGHT 7

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

uniform DirLight dirLight;
uniform PointLight plight[NR_POINT_LIGHT];

uniform sampler2D texture_diffuse;
uniform sampler2D shadowmap;
uniform sampler2D shadowmap2;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir,vec3 color);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec3 color);


float ShadowCalculation(PointLight light,vec4 fragPosLightSpace,sampler2D shadowmap)
{
    float shadow=0;
    vec3 lightDir = normalize(light.position - FragPos);
    float bias =max(0.005 * (1.0 - dot(Normal, lightDir)), 0.002);
    vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
 
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowmap, projCoords.xy).r; 
 
    float currentDepth = projCoords.z;
        if (currentDepth > 1.0) {
        currentDepth = 0.0;
    }
    // PCF
   for(int i=-4;i<=4;++i)
   {
       for(int j=-4;j<=4;j++)
       {
           float closestDepth = texture(shadowmap, projCoords.xy+vec2(i,j)*texelSize).r; 
           shadow += currentDepth-bias > closestDepth  ? 1.0 : 0.0;
       }
   }

    return shadow/81;
}

void main()
{    
    float shadow = ShadowCalculation(plight[6],FragPosLightSpace,shadowmap);
   // float shadow2 = ShadowCalculation(plight[1],FragPosLightSpace,shadowmap);
    shadow = min(shadow,0.85f);
    vec3 color = texture(texture_diffuse, TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 normal = normalize(Normal);


 
    vec3 result = CalcDirLight(dirLight, normal,  viewDir, color);
    for(int i =0;i<NR_POINT_LIGHT;i++)
        result +=CalcPointLight(plight[i],normal,FragPos,viewDir,color);

    FragColor = vec4((1.0f-shadow)*result, 1.0f);
}


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir,vec3 color)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
  
    // combine results
    vec3 ambient = light.ambient * color*0.2f;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec3 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
    
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * color*0.5f;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + (diffuse + specular));
}




