#include <include\GLEW\glew.h>
#include <include\GLFW\glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type,const std::string& source)
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
        std::cout << "Compile "<<((type == GL_VERTEX_SHADER)?"vertex":"fragment") << " Shader Error on:\n"
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

int main(int argc,char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glew和glad都需要在初始化一个有效的上下文之后再进行创建
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error on glewInit();" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float position[6] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    //创建Buffer索引
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //指定使用索引 1 Buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //为该buffer分配空间,并声明为Static类型
    glBufferData(GL_ARRAY_BUFFER,6 * sizeof(float),position,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    
    //指明初始索引、数据量、数据类型、是否需要标准化（改为-1.0~1.0之间）、顶点属性总偏移量
    //最后一个参数是：当前顶点属性在缓冲区中相对于起始位置的偏移量（在一个顶点中，比如前8字节为位置，后面12字节为颜色，则需设为(void*)8）
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color=vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader,fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //drawcall 之一，无需索引缓存
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}