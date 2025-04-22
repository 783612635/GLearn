#include <include\GLEW\glew.h>
#include <include\GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//���Ե��� ΢���ṩ��__debugbreak()
#define ASSERT(x)  if(!(x)) __debugbreak(); 
//#x��������ת��Ϊ�ַ���
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//�˴�ֻ�����GL�������ǲ�����Ҫ���ش�����
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);//while (!glGetError());GL_NO_ERRORΪ0
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error](" << error << ");" << function
            << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParserShader(const std::string& filePath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

//int main(int argc, char** argv)
//{
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    // ���� OpenGL 3.3 ���������ļ�
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    glfwSwapInterval(1);//������ֱͬ��
//
//    //glew��glad����Ҫ�ڳ�ʼ��һ����Ч��������֮���ٽ��д���
//    if (glewInit() != GLEW_OK)
//    {
//        std::cout << "Error on glewInit();" << std::endl;
//    }
//
//    std::cout << glGetString(GL_VERSION) << std::endl;
//
//    float position[8] = {
//        -0.5f, -0.5f,       //0
//         0.5f, -0.5f,       //1
//         0.5f,  0.5f,       //2
//        -0.5f,  0.5f,       //3
//    };
//
//    unsigned int indices[] = {
//        0,1,2,
//        2,3,0
//    };
//
//    unsigned int vao;
//    GLCall(glGenVertexArrays(1,&vao));
//    GLCall(glBindVertexArray(vao));
//
//    //����Buffer����
//    unsigned int buffer;
//    GLCall(glGenBuffers(1, &buffer));
//    //ָ��ʹ������ 1 Buffer
//    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
//    //Ϊ��buffer����ռ�,������ΪStatic����
//    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(unsigned int), position, GL_STATIC_DRAW));
//
//    GLCall(glEnableVertexAttribArray(0));
//
//    //ָ����ʼ���������������������͡��Ƿ���Ҫ��׼������Ϊ-1.0~1.0֮�䣩������������ƫ����
//    //���һ�������ǣ���ǰ���������ڻ��������������ʼλ�õ�ƫ��������һ�������У�����ǰ8�ֽ�Ϊλ�ã�����12�ֽ�Ϊ��ɫ��������Ϊ(void*)8��
//    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
//
//    //������������
//    unsigned int ibo;
//    GLCall(glGenBuffers(1, &ibo));
//    //ָ��ʹ������ 1 Buffer
//    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
//    //Ϊ��buffer����ռ�,������ΪStatic����
//    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW));
//
//    //��������ɳ����·��������Դ�����
//    ShaderProgramSource source = ParserShader("../../GLearn/GLearn/res/shaders/Basic.shader");
//
//    //std::cout << "VERTEX:\n" << source.vertexSource << "\nFRAGMENT:\n" << source.fragmentSource << std::endl;
//
//    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
//
//    GLCall(glUseProgram(shader));
//
//    //��ʹ��һ���ڽ���ͳһ����ǰ����������Ҫ�����ǻ�ȡ���������λ��
//    //ÿ��ͳһ��������һ��id���������ǾͿ�����������
//    //gl4.3��ʼ������ָ����ȷ��ͳһ����λ��
//    int location = glGetUniformLocation(shader, "u_Color");
//    ASSERT(location != -1);
//    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));
//
//    //unBind
//    GLCall(glUseProgram(0));
//    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//    GLCall(glBindVertexArray(0));
//    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//
//    float r = 0.0f;
//    float increment = 0.05f;
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /* Render here */
//        GLCall(glClear(GL_COLOR_BUFFER_BIT));
//
//        //�����ڻ���ǰ�����°�
//        GLCall(glUseProgram(shader));
//        GLCall(glBindVertexArray(vao));
//        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
//        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
//
//        GLCall(glEnableVertexAttribArray(0));
//        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
//
//        //GLClearError();
//        ////drawcall ֮һ��������������
//        ////glDrawArrays(GL_TRIANGLES, 0, 3);
//        //glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);//���һ��������nullptr��ԭ���ǣ�����glBindBuffer�Ѿ���ibo����
//        ////GLCheckError();
//        //ASSERT(GLLogCall());
//        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
//        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//
//        if (r > 1.0f)
//            increment = -0.05f;
//        else if (r < 0.0f)
//            increment = 0.05f;
//
//        r += increment;
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//    GLCall(glDeleteProgram(shader));
//
//END:
//    glfwTerminate();
//    return 0;
//}