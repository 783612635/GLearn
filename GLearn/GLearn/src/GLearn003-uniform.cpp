#include <include\GLEW\glew.h>
#include <include\GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//断言调用 微软提供的__debugbreak()
#define ASSERT(x)  if(!(x)) __debugbreak(); 
//#x将函数名转换为字符串
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//此处只是清除GL错误，我们并不需要返回错误码
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);//while (!glGetError());GL_NO_ERROR为0
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
    //这个函数虽然可以依靠元组来返回多个返回值，但是这并不推荐，所以我们用自定义结构体来解决
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

//int main(int argc, char** argv)
//{
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    // 请求 OpenGL 3.3 核心配置文件
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
//    glfwSwapInterval(1);//开启垂直同步
//
//    //glew和glad都需要在初始化一个有效的上下文之后再进行创建
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
//    //创建Buffer索引
//    unsigned int buffer;
//    GLCall(glGenBuffers(1, &buffer));
//    //指定使用索引 1 Buffer
//    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
//    //为该buffer分配空间,并声明为Static类型
//    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(unsigned int), position, GL_STATIC_DRAW));
//
//    GLCall(glEnableVertexAttribArray(0));
//
//    //指明初始索引、数据量、数据类型、是否需要标准化（改为-1.0~1.0之间）、顶点属性总偏移量
//    //最后一个参数是：当前顶点属性在缓冲区中相对于起始位置的偏移量（在一个顶点中，比如前8字节为位置，后面12字节为颜色，则需设为(void*)8）
//    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
//
//    //创建索引缓存
//    unsigned int ibo;
//    GLCall(glGenBuffers(1, &ibo));
//    //指定使用索引 1 Buffer
//    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
//    //为该buffer分配空间,并声明为Static类型
//    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW));
//
//    //相对于生成程序的路径，不是源代码的
//    ShaderProgramSource source = ParserShader("../../GLearn/GLearn/res/shaders/Basic.shader");
//
//    //std::cout << "VERTEX:\n" << source.vertexSource << "\nFRAGMENT:\n" << source.fragmentSource << std::endl;
//
//    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
//
//    GLCall(glUseProgram(shader));
//
//    //在使用一个内建的统一变量前，我们首先要做的是获取这个变量的位置
//    //每个统一变量都有一个id，这样我们就可以引用它了
//    //gl4.3开始，可以指定明确的统一变量位置
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
//        //解绑后，在绘制前，重新绑定
//        GLCall(glUseProgram(shader));
//        GLCall(glBindVertexArray(vao));
//        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
//        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
//
//        GLCall(glEnableVertexAttribArray(0));
//        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
//
//        //GLClearError();
//        ////drawcall 之一，无需索引缓存
//        ////glDrawArrays(GL_TRIANGLES, 0, 3);
//        //glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);//最后一个参数填nullptr的原因是，上面glBindBuffer已经将ibo绑定了
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