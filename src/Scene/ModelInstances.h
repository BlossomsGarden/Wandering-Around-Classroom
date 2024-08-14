#pragma once
#include "Model.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class ModelInstances
{
public:
	unsigned int instanceVBO;
	int m_InstanceNum;
	std::vector<glm::mat4> model;
	ModelInstances(int InstatnceNum,std::vector<glm::mat4> & model):m_InstanceNum(InstatnceNum), model(model),instanceVBO(0){};
	ModelInstances(int InstatnceNum) :m_InstanceNum(InstatnceNum),instanceVBO(0) {};
	ModelInstances() :m_InstanceNum(0) ,instanceVBO(0){};


	void AddInstanceModel(glm::mat4 model) {
		this->model.push_back(model);
	};


	void SetUpInstancing(Model&instance);
	void Draw(Model& model,Shader& shader);
};

