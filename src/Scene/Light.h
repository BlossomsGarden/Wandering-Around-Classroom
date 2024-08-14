#pragma once
#include <glm/glm.hpp>
#include "../RenderCore/Shader.h"
#include "ShadowMap.h"




class DirLight {
public:
	glm::vec3 m_Direction;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	DirLight();;
	DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void loadLight(Shader& shader);
	void loadEmpty(Shader& shader);
	void set(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

class PointLight {
private :
	unsigned int index;
public:

	glm::vec3 m_Pos;
	glm::vec3 m_Lightcolor;

	

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float constant;
	float linear;
	float quadratic;

	unsigned int m_DepthMap=0;
	unsigned int m_FBO=0;
	bool hasShadowMap = true;

	PointLight(unsigned int index);;
	PointLight(unsigned int index, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

	PointLight(unsigned int index, glm::vec3 pos, glm::vec3 lightcolor, float ambient_ratio, float diffuse_ratio, float specular_ratio, float constant, float linear, float quadratic);

	PointLight(unsigned int index, glm::vec3 pos, PointLight& Template);

	void loadEmpty(Shader& shader);
	void loadLight(Shader& shader);



	void Draw(Shader& LightShader);
	void set(glm::vec3 pos, glm::vec3 lightcolor, float ambient_ratio, float diffuse_ratio, float specular_ratio, float constant, float linear, float quadratic);
	void set(glm::vec3 pos,  PointLight& Template);

	void SetUpShadowMap(int shadow_width,int shadow_height,float m_near,float m_far);

	void RenderShadowMap(Shader& shadowShader);;


};



class SpotLight {



};


struct LightMask {
	bool dirlight_on = true;
	std::vector<bool> plight_on;
};





/*TODO:Can be a singleton*/
class LightManager {
private:
	const unsigned int NR_Pointlights;
	LightMask m_Mask;
public:
	DirLight m_Dirlight;
	
	std::vector<PointLight> m_Pointlights;
	
	
	LightManager(const unsigned int NR_Pointlights);;
	void SetDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);;
	void SetPointLight(unsigned int index, glm::vec3 pos, glm::vec3 lightcolor, float ambient_ratio, float diffuse_ratio, float specular_ratio, float constant, float linear, float quadratic);;
	void SetPointLight(unsigned int index, glm::vec3 pos, PointLight& Template);
	
	void DirLightStat(bool is_on);
	void PointLightStat(unsigned int index,bool is_on);


	/*Set Perspective and view mat before casting light*/
	void CastLight(Shader& shader);;
	void ShowPointLight(Shader& lightshader);

	void SetUpShadowMap(unsigned int index, int shadow_width, int shadow_height, float m_near, float m_far);


	void SetUpShadowMap(int shadow_width, int shadow_height, float m_near, float m_far);
};