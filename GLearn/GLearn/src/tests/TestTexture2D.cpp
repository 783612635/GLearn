#include "TestTexture2D.h"
#include "../Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
		:m_TranslationA{0,0,0}
		,m_TranslationB{200, 100, 0}
		,m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f))
		,m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		//顶点坐标和纹理坐标
		float position[16] = {
			-100.0f, -100.0f, 0.0f, 0.0f,     //0
			 100.0f, -100.0f, 1.0f, 0.0f,     //1
			 100.0f,  100.0f, 1.0f, 1.0f,     //2
			-100.0f,  100.0f, 0.0f, 1.0f,     //3
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0,
		};

		//启用透明混合
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		m_VAO = std::make_unique<VertexArray>();
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		m_VertexBuffer = std::make_unique<VertexBuffer>(position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		//位置属性
		layout.Push<float>(2);
		//纹理属性
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader = std::make_unique<Shader>("../../GLearn/GLearn/res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>("../../GLearn/GLearn/res/textures/fiona.jpg");
		m_Shader->SetUniform1i("u_Texture", 0);//指定OpenGL应该从哪个插槽取纹理数据
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Shader->Bind();
		m_Texture->Bind();

		//每帧重新计算
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("translationA", &m_TranslationA.x, 0.0f, 1980.0f);
		ImGui::SliderFloat3("translationB", &m_TranslationB.x, 0.0f, 1980.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}