#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(const std ::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	//	Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name,float v0,float v1,float v2,float v3);
	void SetUniformVec4f(const std::string& name, const glm::vec4& vec);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);

private:
	ShaderProgramSource ParserShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);

private:
	std::string m_Filepath;

	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
};