#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../RenderCore/VertexArray.h"
#include "../RenderCore/VertexBuffer.h"
#include "../RenderCore/VertexBufferLayout.h"
#include "../utils.h"



PointLight::PointLight(unsigned int index, glm::vec3 pos, PointLight& Template) :index(index), m_Pos(pos)
{
	this->m_Ambient = Template.m_Ambient;
	this->m_Diffuse = Template.m_Diffuse;
	this->m_Specular = Template.m_Specular;
	this->m_Lightcolor = Template.m_Lightcolor;
	this->constant = Template.constant;
	this->linear = Template.linear;
	this->quadratic = Template.quadratic;
}

PointLight::PointLight(unsigned int index, glm::vec3 pos, glm::vec3 lightcolor, float ambient_ratio, float diffuse_ratio, float specular_ratio, float constant, float linear, float quadratic) 
	:index(index), m_Pos(pos), constant(constant), linear(linear), quadratic(quadratic), m_Lightcolor(lightcolor)
{
	m_Ambient = lightcolor * glm::vec3(ambient_ratio);
	m_Diffuse = lightcolor * diffuse_ratio;
	m_Specular = lightcolor * specular_ratio;
}

PointLight::PointLight(unsigned int index, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) 
	:index(index), m_Ambient(ambient), m_Diffuse(diffuse), m_Pos(pos), m_Specular(specular), constant(constant),
linear(linear), quadratic(quadratic), m_Lightcolor(glm::vec3(1.0f, 1.0f, 1.0f))
{

}

PointLight::PointLight(unsigned int index)
{
	this->index = index;
}

void PointLight::loadEmpty(Shader& shader)
{
	shader.Bind();
	shader.SetVec3("plight[" + std::to_string(index) + "].position", m_Pos);
	shader.SetVec3("plight[" + std::to_string(index) + "].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.SetVec3("plight[" + std::to_string(index) + "].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.SetVec3("plight[" + std::to_string(index) + "].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.SetFloat("plight[" + std::to_string(index) + "].constant", constant);
	shader.SetFloat("plight[" + std::to_string(index) + "].linear", linear);
	shader.SetFloat("plight[" + std::to_string(index) + "].quadratic", quadratic);
	shader.Unbind();
}

void PointLight::loadLight(Shader& shader)
{
	shader.Bind();
	shader.SetVec3("plight[" + std::to_string(index) + "].position", m_Pos);
	shader.SetVec3("plight[" + std::to_string(index) + "].ambient", m_Ambient);
	shader.SetVec3("plight[" + std::to_string(index) + "].diffuse", m_Diffuse);
	shader.SetVec3("plight[" + std::to_string(index) + "].specular", m_Specular);
	shader.SetFloat("plight[" + std::to_string(index) + "].constant", constant);
	shader.SetFloat("plight[" + std::to_string(index) + "].linear", linear);
	shader.SetFloat("plight[" + std::to_string(index) + "].quadratic", quadratic);
	shader.Unbind();
}

void PointLight::Draw(Shader& LightShader)
{
	VertexBuffer VBO(CubeVertices,sizeof(CubeVertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexArray VAO;
	VAO.AddBuffer(VBO, layout);

	glm::mat4 model(1.0f);
	model = glm::translate(model,this->m_Pos);
	model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
	LightShader.Bind();
	LightShader.SetMat4("model", model);
	LightShader.SetVec3("gColor", m_Lightcolor);


	VAO.Bind();
	LightShader.Bind();

	GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
	VAO.Unbind();
	LightShader.Unbind();

}

void PointLight::set(glm::vec3 pos, PointLight& Template)
{
	m_Pos = pos;
	this->m_Lightcolor = Template.m_Lightcolor;
	this->m_Ambient = Template.m_Ambient;
	this->m_Diffuse = Template.m_Diffuse;
	this->m_Specular = Template.m_Specular;
	this->m_Lightcolor = Template.m_Lightcolor;
	this->constant = Template.constant;
	this->linear = Template.linear;
	this->quadratic = Template.quadratic;
}

void PointLight::set(glm::vec3 pos, glm::vec3 lightcolor, float ambient_ratio, float diffuse_ratio, float specular_ratio, float constant, float linear, float quadratic)
{
	m_Pos = pos;
	m_Lightcolor = lightcolor;
	m_Ambient = lightcolor * ambient_ratio;
	m_Diffuse = lightcolor * diffuse_ratio;
	m_Specular = lightcolor * specular_ratio;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void PointLight::SetUpShadowMap(int shadow_width, int shadow_height, float m_near, float m_far)
{
	if (hasShadowMap) {
		ShadowMap shadowmap(shadow_width, shadow_height, m_near, m_far);
		m_DepthMap = shadowmap.GetDepthMap();
		m_FBO = shadowmap.GetFBO();
	}
}

void PointLight::RenderShadowMap(Shader& shadowShader)
{
	




}

LightManager::LightManager(const unsigned int NR_Pointlights) 
	:NR_Pointlights(NR_Pointlights)
{
	for (unsigned int i = 0; i < NR_Pointlights; i++)
	{
		m_Pointlights.push_back(PointLight(i));
		m_Mask.plight_on.push_back(false);
	}
}

void LightManager::SetDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_Mask.dirlight_on = true;
	m_Dirlight.set(direction, ambient, diffuse, specular);
}

void LightManager::SetPointLight(unsigned int index, glm::vec3 pos, glm::vec3 lightcolor, float ambient_ratio, float diffuse_ratio, float specular_ratio, float constant, float linear, float quadratic)
{
	
	if (index < NR_Pointlights) {
		m_Pointlights[index].set(pos, lightcolor, ambient_ratio, diffuse_ratio, specular_ratio, constant, linear, quadratic);
		//if you set it, you use it
		m_Mask.plight_on[index] = true;
	}
	else {
		std::cout << "LightManager:Index out of boundary when setting point light.";
	}
}

void LightManager::SetPointLight(unsigned int index, glm::vec3 pos, PointLight& Template)
{
	if (index < NR_Pointlights) {
		m_Pointlights[index].set(pos, Template);
		//if you set it, you use it
		m_Mask.plight_on[index] = true;
	}
	else {
		std::cout << "LightManager:Index out of boundary when setting point light.";
	}
}

void LightManager::DirLightStat(bool is_on)
{
	m_Mask.dirlight_on = is_on;
}

void LightManager::PointLightStat(unsigned int index, bool is_on)
{
	if (index < NR_Pointlights)
		m_Mask.plight_on[index] = is_on;
}

void LightManager::CastLight(Shader& shader)
{
	if (m_Mask.dirlight_on)
		m_Dirlight.loadLight(shader);
	for (int i = 0; i <(int) NR_Pointlights; i++)
	{
		if (m_Mask.plight_on[i]) {
			m_Pointlights[i].loadLight(shader);
			
		}
		else {
			m_Pointlights[i].loadEmpty(shader);
		}
	}
}

void LightManager::ShowPointLight(Shader& lightshader)
{
	for (int i = 0; i <(int)NR_Pointlights; i++) {
		if (m_Mask.plight_on[i])
			m_Pointlights[i].Draw(lightshader);
	}
}

void LightManager::SetUpShadowMap(unsigned int index, int shadow_width, int shadow_height, float m_near, float m_far)
{
	if (index < NR_Pointlights)
	{
		this->m_Pointlights[index].hasShadowMap = true;
		this->m_Pointlights[index].SetUpShadowMap(shadow_width, shadow_height, m_near, m_far);

	}
}
void LightManager::SetUpShadowMap( int shadow_width, int shadow_height, float m_near, float m_far)
{
	for(int i =0;i<(int)NR_Pointlights;i++)
	{
		this->m_Pointlights[i].hasShadowMap = true;
		this->m_Pointlights[i].SetUpShadowMap(shadow_width, shadow_height, m_near, m_far);

	}
}


DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	:m_Ambient(ambient), m_Diffuse(diffuse), m_Direction(direction), m_Specular(specular)
{

}

DirLight::DirLight()
{

}

void DirLight::loadLight(Shader& shader)
{
	shader.Bind();
	shader.SetVec3("dirLight.direction", m_Direction);
	shader.SetVec3("dirLight.ambient", m_Ambient);
	shader.SetVec3("dirLight.diffuse", m_Diffuse);
	shader.SetVec3("dirLight.specular", m_Specular);
	shader.Unbind();
}

void DirLight::loadEmpty(Shader& shader)
{
	shader.Bind();
	shader.SetVec3("dirLight.direction", m_Direction);
	shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.SetVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.SetVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.Unbind();
}

void DirLight::set(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_Direction = direction;
	m_Ambient = ambient;
	m_Diffuse = diffuse;
	m_Specular = specular;
}
