#include "Renderer.h"
#include <iostream>
#include "VertexBufferLayout.h"
#include "../utils.h"
#include "../Global.h"
#include "Texture.h"

void Renderer::UseTextureCache(TextureCache& T) 
{ 
	m_Textures = T;
	T.Bind();
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader,GLenum mode) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCALL(glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, nullptr));


	/*unbind*/

	shader.Unbind();
	va.Unbind();
	ib.Unbind();



}

void Renderer::Draw(const EntityObject3D& Object, Shader&shader,GLenum mode) const
{
	//Texture texture(DEFAULT_TEXTURE_PATH);
	//texture.Bind(Object.GetTextureSlot());
	//by default 0

	auto it = m_Textures.m_TexCache.find(Object.GetTextureSlot());
	if (it != m_Textures.m_TexCache.end())
		it->second.Bind();

	shader.Bind();
	shader.SetInt("u_Texture", Object.GetTextureSlot());
	glm::mat4 model(1.0f);
	model = glm::translate(model, Object.GetPos());
	model = glm::rotate(model,glm::radians(Object.GetRotateAngle()),Object.GetRotateAxis());
	model = glm::scale(model, glm::vec3(Object.GetScale()));
	shader.SetMat4("model", model);

	
	//**
	VertexBuffer VBO(&Object.GetVertices()[0], sizeof(float) * Object.GetVertices().size());
	VBO.Bind();
	VertexArray VAO;

	VertexBufferLayout layout;
	layout.Push<float>(3);//xyz
	layout.Push<float>(3);//norm
	layout.Push<float>(2);//tex_coord
	VAO.AddBuffer(VBO, layout);


	IndexBuffer IBO(&Object.GetIndices()[0], Object.GetIndexNum());
	VAO.Bind();
	IBO.Bind();


	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	GLCALL(glDrawElements(mode,Object.GetIndexNum(),GL_UNSIGNED_INT,nullptr ));




	shader.Unbind();
}

//clear mode
void Renderer::Clear(glm::vec4 color) const
{
	glClearColor(color.r,color.g,color.b,color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
