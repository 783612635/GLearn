#include "TestDynamicalRendering.h"
#include "../Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>

struct vec3
{
	float x,y,z;
};

//�����floatӦ���滻Ϊ�����Զ���Ľṹ��
struct Vertex
{
	float Position[3];
	float Color[4];
	float TexCoords[2];
	float TexID;
};

test::TestDynamicalRendering::TestDynamicalRendering()
	:m_Translation{ 0,0,0 }
	, m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f))
	, m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
	//�������沢����Ҫ�ı�
	unsigned int indices[] = {
		0,1,2,
		2,3,0,
		4,5,6,
		6,7,4
	};

	//����͸�����
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	m_VAO = std::make_unique<VertexArray>();
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);
	m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex)*1000);

	//GLCall(glEnableVertexAttribArray(0));
	//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,Position)));

	//GLCall(glEnableVertexAttribArray(1));
	//GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	layout.Push<float>(2);
	layout.Push<float>(1);
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_Shader = std::make_unique<Shader>("../../GLearn/GLearn/res/shaders/BatchDynamics.shader");
	m_Shader->Bind();
	m_Texture_Fiona = std::make_unique<Texture>("../../GLearn/GLearn/res/textures/fiona.jpg");
	m_Texture_Tifa = std::make_unique<Texture>("../../GLearn/GLearn/res/textures/tifa.jpg");

	int samplers[2] = { 0,1 };
	m_Shader->SetUniform1iv("u_Textures", 2, samplers);
}

test::TestDynamicalRendering::~TestDynamicalRendering()
{
}

void test::TestDynamicalRendering::OnUpdate(float deltaTime)
{
}

static std::array<Vertex, 4> CreateQuad(float x, float y,float size,float textureID)
{
	Vertex v0 = {
		{ x,y,0.0f},
		{ 0.18f,0.6f,0.96f,1.0f },
		{ 0.0f, 0.0f },
		textureID
	};

	Vertex v1 = {
		{ x+size,y,	0.0f},
		{ 0.18f,0.6f,0.96f,1.0f },
		{  1.0f, 0.0f },
		textureID
	};

	Vertex v2 = {
		{ x+size,	y+size,	0.0f},
		{ 0.18f,0.6f,0.96f,1.0f },
		{  1.0f, 1.0f },
		textureID
	};

	Vertex v3 = {
		{ x,	y+size,	0.0f},
		{ 0.18f,0.6f,0.96f,1.0f },
		{  0.0f, 1.0f },
		textureID
	};

	return { v0,v1,v2,v3 };
}

void test::TestDynamicalRendering::OnRender()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	// ��������ǰ���������������飬��������������������ж��
	// ʵ�ʲμ���Ⱦ������ֻ��ǰ8������
//float position[] = {
//	-100.0f,	-100.0f,	0.0f,		0.18f,0.6f,0.96f,1.0f,	 0.0f, 0.0f, 	0.0f,	//0
//	 100.0f,	-100.0f,	0.0f,		0.18f,0.6f,0.96f,1.0f,	 1.0f, 0.0f, 	0.0f,	//1
//	 100.0f,	 100.0f,	0.0f,		0.18f,0.6f,0.96f,1.0f,	 1.0f, 1.0f, 	0.0f,	//2
//	-100.0f,	 100.0f,	0.0f,		0.18f,0.6f,0.96f,1.0f,	 0.0f, 1.0f, 	0.0f,	//3
//	
//	100.0f,100.0f,0.0f,		1.0f,0.93f,0.24f,1.0f,		0.0f, 0.0f, 	1.0f,	//4
//	200.0f,100.0f,0.0f,		1.0f,0.93f,0.24f,1.0f,		1.0f, 0.0f, 	1.0f,	//5
//	200.0f,200.0f,0.0f,		1.0f,0.93f,0.24f,1.0f,		1.0f, 1.0f, 	1.0f,	//6
//	100.0f,200.0f,0.0f,		1.0f,0.93f,0.24f,1.0f,		0.0f, 1.0f, 	1.0f	//7
//};

	auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 200.0f, 0.0f);
	auto q1 = CreateQuad(200.0f, 200.0f,100.0f,1.0f);

	Vertex vectices[8];
	memcpy(vectices, q0.data(), q0.size() * sizeof(Vertex));
	memcpy(vectices+q0.size(), q1.data(), q1.size() * sizeof(Vertex));

	Renderer renderer;
	m_Shader->Bind();
	m_VertexBuffer->Bind();
	// ��̬�ؽ��������ݴ��ڴ洫�䵽GPU��
	// ����֮һ��ʹ��glMapBuffer,ʹ��������һ��ָ���ڴ�Ŀ�ָ��������д�룬Ȼ���ٵ���glUnMapBuffer
	// ������������Ҫ��һЩ
	//GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(position),position));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vectices), vectices));

	m_Texture_Fiona->Bind(0);
	m_Texture_Tifa->Bind(1);

	//ÿ֡���¼���
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}
}

void test::TestDynamicalRendering::OnImGuiRender()
{
	ImGui::SliderFloat3("translation", &m_Translation.x, 0.0f, 1980.0f);
	ImGui::DragFloat("Quad position", m_QuadPosition, 1.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
