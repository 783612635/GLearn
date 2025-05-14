#include "TestBatchTextures.h"
#include "../Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

test::TestBatchTextures::TestBatchTextures()
	:m_Translation{ 0,0,0 }
	, m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f))
	, m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
	//每个顶点最后一个数据代表纹理索引
	float position[] = {
	-100.0f, -100.0f,0.0f, 0.0f, 	0.0f,	//0
	 100.0f, -100.0f,1.0f, 0.0f, 	0.0f,	//1
	 100.0f,  100.0f,1.0f, 1.0f, 	0.0f,	//2
	-100.0f,  100.0f,0.0f, 1.0f, 	0.0f,	//3

	100.0f, 100.0f,	 0.0f, 0.0f, 	1.0f,	//4
	200.0f, 100.0f,	 1.0f, 0.0f, 	1.0f,	//5
	200.0f,  200.0f,1.0f, 1.0f, 	1.0f,	//6
	100.0f,  200.0f,0.0f, 1.0f, 	1.0f	//7
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0,
		4,5,6,
		6,7,4
	};

	//启用透明混合
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	m_VAO = std::make_unique<VertexArray>();
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);
	m_VertexBuffer = std::make_unique<VertexBuffer>(position, 2 * 4 * 5 * sizeof(float));
	VertexBufferLayout layout;
	//位置属性
	layout.Push<float>(2);
	layout.Push<float>(2);
	layout.Push<float>(1);
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_Shader = std::make_unique<Shader>("../../GLearn/GLearn/res/shaders/BatchTexture.shader");
	m_Shader->Bind();
	m_Texture_Fiona = std::make_unique<Texture>("../../GLearn/GLearn/res/textures/fiona.jpg");
	m_Texture_Tifa = std::make_unique<Texture>("../../GLearn/GLearn/res/textures/tifa.jpg");

	int samplers[2] = { 0,1 };
	m_Shader->SetUniform1iv("u_Textures",2,samplers);
}

test::TestBatchTextures::~TestBatchTextures()
{
}

void test::TestBatchTextures::OnUpdate(float deltaTime)
{
}

void test::TestBatchTextures::OnRender()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	Renderer renderer;
	m_Shader->Bind();
	m_Texture_Fiona->Bind(0);
	m_Texture_Tifa->Bind(1);

	//每帧重新计算
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}
}

void test::TestBatchTextures::OnImGuiRender()
{
	ImGui::SliderFloat3("translation", &m_Translation.x, 0.0f, 1980.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
