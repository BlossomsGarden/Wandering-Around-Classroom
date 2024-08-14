#include "SkyBox.h"
#include "../RenderCore/Renderer.h"
#include "../RenderCore/VertexBufferLayout.h"
#include <stb/stb_image.h>
#include "../utils.h"


SkyBox::SkyBox(const std::string& Path) :m_RendererID(0)
{

	faces.push_back(Path + "/right.jpg");
	faces.push_back(Path + "/left.jpg");
	faces.push_back(Path + "/top.jpg");
	faces.push_back(Path + "/bottom.jpg");
	faces.push_back(Path + "/front.jpg");
	faces.push_back(Path + "/back.jpg");
	loadCubemap();
}

void SkyBox::Draw(Shader& shader,unsigned int texture_slot/* =0 */)
{
	GLCALL(glDepthFunc(GL_LEQUAL));
	VertexBuffer VBO(CubeVertices, sizeof(CubeVertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexArray VAO;
	VAO.AddBuffer(VBO, layout);

	VAO.Bind();

	GLCALL(glActiveTexture(GL_TEXTURE0+texture_slot));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
	shader.Bind();
	shader.SetInt("skybox",  texture_slot);

	GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
	VAO.Unbind();
	shader.Unbind();
	GLCALL(glDepthFunc(GL_LESS));
}

void SkyBox::loadCubemap()
{
	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
}
