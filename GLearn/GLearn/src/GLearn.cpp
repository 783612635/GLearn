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

    glfwTerminate();
    return 0;
}