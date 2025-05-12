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
    //���������Ȼ��������Ԫ�������ض������ֵ�������Ⲣ���Ƽ��������������Զ���ṹ�������
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    //��ɫ������ľ����Դ������Դ��
    //��գ���ζ���ַ����Կս�β;
    //����ǿգ�Ӧ��ָ��һ�����飬����ÿ����ӦԪ�ص��ַ������Ȼ�����С��0��ֵ��ʾ�Կս�β
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
    //���shader�����Ƿ������ڵ�ǰopengl��״̬
    glValidateProgram(program);

    //����shader�Ƿ��Ѿ��ɹ����ӵ������У��˿̶�Ӧ��ɾ��������м���ȵȱ������
    //glDetachShader();����ɾ��Դ�룬���ǲ�̫���ã���ΪԴ��ռ�ÿռ�Ƚ�С����ͨ�����԰������ǽ��е���
    glDeleteShader(vs);
    glDeleteShader(vs);

    return program;
}

//��ȡ����ͳһ������λ�û��������������ε���glGetUniformLocation����Ч��
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
    //v ��˼�Ǵ���һ������
    GLCall(glUniformMatrix4fv(GetUniformLocation(name),1, GL_FALSE,&mat[0][0]));
}
