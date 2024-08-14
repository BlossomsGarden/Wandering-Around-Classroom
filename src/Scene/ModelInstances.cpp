#include "ModelInstances.h"
#include "../utils.h"

//Set Model mat and attribptr for instance 
void ModelInstances::SetUpInstancing(Model& instance)
{

	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model.size(), &model[0], GL_STATIC_DRAW));
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	for (unsigned int i = 0; i < instance.meshes.size(); i++)
	{
		unsigned int VAO = instance.meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

}

void ModelInstances::Draw(Model& model, Shader& shader)
{
	model.Draw(shader, this->m_InstanceNum);
}
