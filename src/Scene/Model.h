#pragma once

#include "../RenderCore/Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
	public :
		Model(char* path) 
		{
			loadModel(path);

		}
		Model(std::string const& path, bool gamma = false) : gammaCorrection(gamma)
		{
			loadModel(path);
		}
		bool gammaCorrection;
		void Draw(Shader& shader);
		void Draw(Shader& shader,int instanceAmount);
		std::vector<Mesh::Mesh> meshes;
	private:
		std::vector<Mesh::Texture> TextureLoaded;
		
		std::string directory;

		void loadModel(std::string path);
		void processNode(aiNode *node,const aiScene* scene);
		Mesh::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* materials, aiTextureType type, std::string typename);



};

