#include <include\GLEW\glew.h>
#include <include\GLFW\glfw3.h>
#include <iostream>

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

    glfwTerminate();
    return 0;
}