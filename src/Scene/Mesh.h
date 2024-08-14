#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string >
#include "../RenderCore/Shader.h"
#include "Material.h"
#include "Vertex.h"

namespace Mesh {

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;


	};


	class Mesh
	{
		public:
			unsigned int VAO;
			unsigned int UBO;
			std::vector<Vertex> vertices;
			std::vector<unsigned int > indices;
			std::vector<Texture> textures;
			Material mat;
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures,Material mats);
			void Draw(Shader &shader);
			void Draw(Shader& shader,int instanceAmount);
	private:
			unsigned int VBO;
			unsigned int IBO;
			
		

			void setUpMesh();

	};

}