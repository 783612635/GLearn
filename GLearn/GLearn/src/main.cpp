#include "Renderer.h"
#include <include\GLFW\glfw3.h>
#include <iostream>
#include "VertexBufferLayout.h"
#include "Textures.h"

int main(int argc, char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // ���� OpenGL 3.3 ���������ļ�
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

    glfwSwapInterval(1);//������ֱͬ��

    //glew��glad����Ҫ�ڳ�ʼ��һ����Ч��������֮���ٽ��д���
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error on glewInit();" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    //�����������������
    float position[16] = {
        -0.5f, -0.5f,0.0f,0.0f,       //0
         0.5f, -0.5f,1.0f,0.0f,     //1
         0.5f,  0.5f, 1.0f,1.0f,    //2
        -0.5f,  0.5f, 0.0f,1.0f    //3
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    //����͸�����
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//���û�Ϻ���
    GLCall(glEnable(GL_BLEND));

    VertexArray va;
    VertexBuffer vb(position, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    //��������ɳ����·��������Դ�����,"../../GLearn/GLearn/res/shaders/Basic.shader"
    Shader shader("../../GLearn/GLearn/res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color",0.4f, 0.3f, 0.8f, 1.0f);

    Texture texture("../../GLearn/GLearn/res/textures/fiona.jpg");
    texture.Bind();
    shader.SetUniform1i("u_Texture",0);//ָ��OpenGLӦ�ô��ĸ����ȡ��������

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