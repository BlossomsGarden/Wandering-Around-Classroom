#include "Shader.h"

#include "../utils.h"
#include "../Global.h"

unsigned int Shader::compileShader(GLenum type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr); /*传入源码段数+相应源码的字符串数组+相应源码字符串长度的数组*/

	glCompileShader(id);
	//ERROR HANDLING#
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(sizeof(char) * length);
		glGetShaderInfoLog(id, length, &length, message);
		std::string shader_type;
		switch (type)
		{
			case GL_VERTEX_SHADER:
				shader_type = "Vertex ";
				break;
			case GL_FRAGMENT_SHADER:
				shader_type = "Fragment ";
				break;
			default:
				shader_type = "";
				break;
		}
		std::cout << "Fail to Compile " << shader_type << "Shader" << std::endl << message << std::endl;
		return  ERROR_COMPILE_SHADER_FAILURE;
	}

	return id;

}



ShaderSource Shader::parseShaderSource(const std::string& filepath ) {
	enum ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::ifstream fin(filepath);
	if (!fin.is_open())
		std::cout << "Fail to load shader source file.";
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;


	while (getline(fin, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;

		}
		else
		{
			if (type != ShaderType::NONE)
				ss[(int)type] << line << '\n';
		}
	}
	return{ ss[ShaderType::VERTEX].str() ,ss[ShaderType::FRAGMENT].str() };
}


unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{

	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));
	GLCALL(glLinkProgram(program));
	GLCALL(glValidateProgram(program));
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));
	return program;
}





Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderSource source = parseShaderSource(filepath);
	m_RendererID = createShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}


void Shader::SetUp(glm::mat4& view,  glm::mat4 &projection)
{
	Bind();
	SetMat4("view",view);
	SetMat4("projection",projection);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCALL(int location = glGetUniformLocation(m_RendererID,name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform " << name << " does not exist." << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SetVec4(const std::string& name, glm::vec4 &value)
{
	GLCALL(glUniform4fv(GetUniformLocation(name),1 ,&value[0]));
}
void Shader::SetVec4(const std::string& name, float v0, float v1,float v2, float v3)
{
	GLCALL(glUniform4f(GetUniformLocation(name), v0,v1,v2,v3));
}
void Shader::SetMat4(const std::string& name, glm::mat4& value)
{
	GLCALL(glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE,&value[0][0]))
}

void Shader::SetFloat(const std::string& name, float value)
{

	GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetInt(const std::string& name, int value)
{
	GLCALL(glUniform1i(GetUniformLocation(name), value));
}



void Shader::SetVec3(const std::string& name, const glm::vec3& value)
{
	GLCALL(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}
