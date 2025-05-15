#include "TestBatchIndexBuffer.h"
#include "../Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>

//下面这段是为了贴合视频，实际上我们可以使用glm::vec*
struct vec2
{
	float x, y;
};

struct vec3
{
	float x, y,z;
};

struct vec4
{
	float x, y,z,w;
};

struct Vertex
{
	vec3 Position;
	vec4 Color;
	vec2 TexCoords;
	float TexID;
};

test::TestBatchIndexBuffer::TestBatchIndexBuffer()
	:m_Translation{ 0,0,0 }
	, m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f))
	, m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
	//unsigned int indices[] = {
	//	0,1,2,
	//	2,3,0,
	//	4,5,6,
	//	6,7,4
	//};

	const size_t maxQuadCount = 1000;
	const size_t maxVertexCount = 4 * maxQuadCount;
	const size_t maxIndexCount = maxQuadCount * 6;

	uint32_t indices[maxIndexCount];
	uint32_t offset = 0;
	for (size_t i = 0; i < maxIndexCount;i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		offset += 4;
	}

	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	m_VAO = std::make_unique<VertexArray>();
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, maxIndexCount);
	m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * maxVertexCount);

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

test::TestBatchIndexBuffer::~TestBatchIndexBuffer()
{
}

void test::TestBatchIndexBuffer::OnUpdate(float deltaTime)
{
}

static Vertex* CreateQuad(Vertex* target,float x, float y, float size, float textureID)
{
	target->Position = { x,y,0.0f };
	target->Color = { 0.18f,0.6f,0.96f,1.0f };
	target->TexCoords = { 0.0f, 0.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x + size,y,	0.0f };
	target->Color = { 0.18f,0.6f,0.96f,1.0f };
	target->TexCoords = { 1.0f, 0.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x + size,	y + size,	0.0f };
	target->Color = { 0.18f,0.6f,0.96f,1.0f };
	target->TexCoords = { 1.0f, 1.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x,	y + size,	0.0f };
	target->Color = { 0.18f,0.6f,0.96f,1.0f };
	target->TexCoords = { 0.0f, 1.0f };
	target->TexID = textureID;
	target++;

	return target;
}

void test::TestBatchIndexBuffer::OnRender()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	uint32_t indexCount = 0;

	std::array<Vertex, 1000> vertices;
	Vertex* buffer = vertices.data();
	for (int y = 0;y < 500;y+=100)
	{
		for (int x = 0;x < 500;x+=100)
		{
			buffer = CreateQuad(buffer, x, y, 100.0f,( x/100+ y/100)% 2);
			indexCount += 6;
		}
	}

	buffer = CreateQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 200.0f,0.0f);
	indexCount += 6;

	Renderer renderer;
	m_Shader->Bind();
	m_VertexBuffer->Bind();

	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data()));

	m_Texture_Fiona->Bind(0);
	m_Texture_Tifa->Bind(1);

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IndexBuffer, indexCount, *m_Shader);
	}
}

void test::TestBatchIndexBuffer::OnImGuiRender()
{
	ImGui::SliderFloat3("translation", &m_Translation.x, 0.0f, 1980.0f);
	ImGui::DragFloat("Quad position", m_QuadPosition, 1.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
