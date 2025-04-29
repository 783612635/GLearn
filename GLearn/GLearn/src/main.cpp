#include "Renderer.h"
#include <include\GLFW\glfw3.h>
#include <iostream>
#include "VertexBufferLayout.h"
#include "Textures.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "Tests\TestClearColor.h"

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
    window = glfwCreateWindow(1920, 1080, "Venus Vacation", NULL, NULL);
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

    //����رմ��ں󣬽������������е����
    {
        //�����������������
        float position[16] = {
            -100.0f, -100.0f, 0.0f, 0.0f,     //0
             100.0f, -100.0f, 1.0f, 0.0f,     //1
             100.0f,  100.0f, 1.0f, 1.0f,     //2
            -100.0f,  100.0f, 0.0f, 1.0f      //3
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

        // ��������:������ӳ�䵽2Dƽ��
        // Ŀ���Ǹ���GL�����ڲ����������Σ����Ǿ��Σ���Ҫ����Ⱦͼ�ν��б任
        glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f,-1.0f,1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        //��CPU��������˷����������Է������ǵ��ԣ���֤���õ��Ľ���Ƿ�����ӳ������-1.0��1.0��
        //glm::vec4 vp(100.0f, 100.0f,0.0f,1.0f);
        //glm::vec4 result = proj * vp;

        //��������ɳ����·��������Դ�����,"../../GLearn/GLearn/res/shaders/Basic.shader"
        Shader shader("../../GLearn/GLearn/res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("../../GLearn/GLearn/res/textures/fiona.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);//ָ��OpenGLӦ�ô��ĸ����ȡ��������

        //unBind
        va.UnBind();
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();

        float r = 0.0f;
        float increment = 0.05f;

        Renderer renderer;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        glm::vec3 translationA(0, 0, 0);
        glm::vec3 translationB(200, 100, 0);

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");

        //��������ʵ��
        //test::TestClearColor test;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            //test.OnUpdate(0.0f);
            //test.OnRender();

            // Start the Dear ImGui frame
            // ��ʼ��֡�󣬲ſ�������ʹ��imgui������
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnRender();
                currentTest->OnUpdate(0.0f);
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<--"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            //test.OnImGuiRender();

            //shader.Bind();

            ////ÿ֡���¼���
            //{
            //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            //    glm::mat4 mvp = proj * view * model;
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    renderer.Draw(va, ib, shader);
            //}

            //{
            //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            //    glm::mat4 mvp = proj * view * model;
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    renderer.Draw(va, ib, shader);
            //}

            //if (r > 1.0f)
            //    increment = -0.05f;
            //else if (r < 0.0f)
            //    increment = 0.05f;

            //r += increment;

            //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            //{
            //    ImGui::Begin("FPS");
            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            //    ImGui::SliderFloat3("translationA", &translationA.x, 0.0f, 1980.0f);
            //    ImGui::SliderFloat3("translationB", &translationB.x, 0.0f, 1980.0f);
            //    ImGui::End();
            //}

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu)
        {
            delete  testMenu;
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}