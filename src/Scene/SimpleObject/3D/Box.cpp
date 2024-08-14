#include "Box.h"
#include "../../../RenderCore/VertexArray.h"
#include "../../../RenderCore/VertexBuffer.h"
#include "../../../RenderCore/VertexBufferLayout.h"
#include "../../../utils.h"
Box::Box(glm::vec3 pos, glm::vec3 scale, Material& mat)
	:mat(mat),pos(pos)
{
	this->model = glm::mat4(1.0f);
	model = glm::translate(model,pos);
	model = glm::scale(model, scale);

}

void Box::Draw(Shader& shader)
{
	VertexBuffer VBO(CubeVertices, sizeof(CubeVertices));
	VertexArray VAO;
	VertexBufferLayout layout;
	layout.Push<float>(3);
	VAO.AddBuffer(VBO, layout);

	shader.Bind();
	unsigned int uniformblockindex = glGetUniformBlockIndex(shader.GetRendererID(),"Mat");




}

