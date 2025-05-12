#include "Shader.h"
#include "Renderer.h"
#include <include\GLEW\glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const std::string& filepath)
	:m_Filepath(filepath)
	,m_RendererID(0)
{
    ShaderProgramSource source = ParserShader(filepath);
    m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParserShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    if (!stream.is_open())
    {
        std::cout << "Error on open Shader files , the path is " << filePath << std::endl;
        return { "","" };
    }

    enum class ShaderType
    {
        ST_None = -1,
        ST_Vertex = 0,
        ST_Frament = 1,
    };

    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::ST_None;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::ST_Vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::ST_Frament;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(),ss[1].str() };
    //这个函数虽然可以依靠元组来返回多个返回值，但是这并不推荐，所以我们用自定义结构体来解决
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    //着色器对象的句柄、源码数、源码
    //填空，意味着字符串以空结尾;
    //如果非空，应该指向一个数组，包含每个相应元素的字符串长度或者以小于0的值表示以空结尾
    //
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //errors handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " Shader Error on:\n"
            << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //GLuint = unsigned int
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    //检查shader代码是否适用于当前opengl的状态
    glValidateProgram(program);

    //不管shader是否已经成功链接到程序中，此刻都应该删除其编译中间件等等编译产物
    //glDetachShader();用以删除源码，但是不太常用，因为源码占用空间比较小，且通常可以帮助我们进行调试
    glDeleteShader(vs);
    glDeleteShader(vs);

    return program;
}

//将取出的统一变量的位置缓存起来，避免多次调用glGetUniformLocation降低效率
unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' dosen's exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0,v1,v2,v3));
}

void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vec)
{
    GLCall(glUniform4fv(GetUniformLocation(name), 1,  &vec[0]));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    //v 意思是传入一个数组
    GLCall(glUniformMatrix4fv(GetUniformLocation(name),1, GL_FALSE,&mat[0][0]));
}
