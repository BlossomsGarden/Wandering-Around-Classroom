#ifndef _PLANE_H
#define _PLANE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../../RenderCore/Shader.h"
#include "../../../RenderCore/Texture.h"
#include "../../../RenderCore/VertexBuffer.h"
#include "../../../RenderCore/VertexBufferLayout.h"
#include "../../../RenderCore/VertexArray.h"
#include "../../../utils.h"
class Plane {
public:
	glm::vec3 m_Pos;
	float m_Width, m_Height;


	VertexArray VAO;
	VertexBuffer VBO;

	Plane(glm::vec3 pos, float Width, float Height, glm::vec3 axis, float angle)
		: m_Pos(pos), m_Height(Height), m_Width(Width)
	{
		VBO.SetData(RectVertices, sizeof(RectVertices));
		VertexBufferLayout layout;

		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		VAO.AddBuffer(VBO, layout);
		VAO.Bind();

	};

	Plane(glm::vec3 pos, float Width, float Height)
		: m_Pos(pos), m_Height(Height), m_Width(Width)
	{
		VBO.SetData(RectVertices, sizeof(RectVertices));
		VertexBufferLayout layout;

		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		VAO.AddBuffer(VBO, layout);
		VAO.Bind();


	};


	void Draw(Shader& shader, Texture& Texture) {
		Texture.Bind();
		shader.Bind();
		shader.SetInt("texture_diffuse", 0);
		VAO.Bind();
		GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));
		VAO.Unbind();
		Texture.Unbind();
	};

	void Draw(Shader& shader, Texture& Texture,unsigned int shadowMap,int shadowMapSlot) {
		Texture.Bind();
		GLCALL(glActiveTexture(GL_TEXTURE0 + shadowMapSlot));
		GLCALL(glBindTexture(GL_TEXTURE_2D, shadowMap));
		shader.Bind();
		shader.SetInt("texture_diffuse", 0);
		shader.SetInt("shadowmap", shadowMapSlot);
		VAO.Bind();
		GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));
		VAO.Unbind();
		GLCALL(glActiveTexture(GL_TEXTURE0 + shadowMapSlot));
		GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
		Texture.Unbind();
	};
	float GetWidth() { return m_Width; };
	float GetHeight() { return m_Height; };
};


#endif
