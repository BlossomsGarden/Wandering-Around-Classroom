#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <iostream>
#include <unordered_map>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>


struct ShaderSource {
	std::string VertexShader;
	std::string FragmentShader;

};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	int GetUniformLocation(const std::string& name);

	std::unordered_map<std::string, int> m_UniformLocationCache;



	unsigned int compileShader(GLenum type, const std::string& source);
	ShaderSource parseShaderSource(const std::string& filepath);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);



public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind()const;
	void Unbind()const;

	void SetUp( glm::mat4& view,  glm::mat4 &projection);


	void SetInt(const std::string& name, int value);
	void SetVec4(const std::string& name, glm::vec4& value);
	void SetVec4(const std::string& name, float v0, float v1, float v2, float v3);
	void SetMat4(const std::string& name, glm::mat4& value);
	void SetFloat(const std::string& name, float value);
	void SetVec3(const std::string& name, const glm::vec3& value);


	unsigned int GetRendererID() const { return m_RendererID; }
};

