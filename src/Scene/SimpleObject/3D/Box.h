#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../../RenderCore/Shader.h"
#include "../../Material.h"
class Box {
private:
	glm::vec3 pos;
	glm::mat4 model;
	Material mat;

public:

	Box(glm::vec3 pos, glm::vec3 scale, Material& mat);
	void Draw(Shader& shader);
};