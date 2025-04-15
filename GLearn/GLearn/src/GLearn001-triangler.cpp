#include <include\GLEW\glew.h>
#include <include\GLFW\glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type,const std::string& source)
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
    //���shader�����Ƿ������ڵ�ǰopengl��״̬
    glValidateProgram(program);

    //����shader�Ƿ��Ѿ��ɹ����ӵ������У��˿̶�Ӧ��ɾ��������м���ȵȱ������
    //glDetachShader();����ɾ��Դ�룬���ǲ�̫���ã���ΪԴ��ռ�ÿռ�Ƚ�С����ͨ�����԰������ǽ��е���
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

    //glew��glad����Ҫ�ڳ�ʼ��һ����Ч��������֮���ٽ��д���
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

    //����Buffer����
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //ָ��ʹ������ 1 Buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //Ϊ��buffer����ռ�,������ΪStatic����
    glBufferData(GL_ARRAY_BUFFER,6 * sizeof(float),position,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    
    //ָ����ʼ���������������������͡��Ƿ���Ҫ��׼������Ϊ-1.0~1.0֮�䣩������������ƫ����
    //���һ�������ǣ���ǰ���������ڻ��������������ʼλ�õ�ƫ��������һ�������У�����ǰ8�ֽ�Ϊλ�ã�����12�ֽ�Ϊ��ɫ��������Ϊ(void*)8��
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

        //drawcall ֮һ��������������
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