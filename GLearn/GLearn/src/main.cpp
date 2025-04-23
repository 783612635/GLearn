#include "Renderer.h"

#include <include\GLFW\glfw3.h>
#include <iostream>
#include "VertexBufferLayout.h"

int main(int argc, char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // 请求 OpenGL 3.3 核心配置文件
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);//开启垂直同步

    //glew和glad都需要在初始化一个有效的上下文之后再进行创建
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error on glewInit();" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float position[8] = {
        -0.5f, -0.5f,       //0
         0.5f, -0.5f,       //1
         0.5f,  0.5f,       //2
        -0.5f,  0.5f,       //3
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    VertexArray va;
    VertexBuffer vb(position, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    //相对于生成程序的路径，不是源代码的,"../../GLearn/GLearn/res/shaders/Basic.shader"
    Shader shader("../../GLearn/GLearn/res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color",0.4f, 0.3f, 0.8f, 1.0f);

    //unBind
    va.UnBind();
    shader.UnBind();
    vb.UnBind();
    ib.UnBind();

    float r = 0.0f;
    float increment = 0.05f;

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        renderer.Draw(va,ib,shader);

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

END:
    glfwTerminate();
    return 0;
}