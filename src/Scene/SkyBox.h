#pragma once
#include "../RenderCore/Texture.h"
#include "../RenderCore/Shader.h"
#include <string>
#include <vector>

class SkyBox
{
public:

	unsigned int m_RendererID;

	//Texture texture;
	SkyBox(const std::string& Path);


	void Draw(Shader& shader,unsigned int texture_slot =0);
private:
	std::vector<std::string> faces;
	void loadCubemap();
	
};

