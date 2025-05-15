#pragma once

#include <memory>

#include "Test.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Textures.h"

namespace test
{
	class TestBatchIndexBuffer :public Test
	{
	public:
		TestBatchIndexBuffer();
		~TestBatchIndexBuffer();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr < VertexArray > m_VAO;
		std::unique_ptr < IndexBuffer > m_IndexBuffer;
		std::unique_ptr < Shader > m_Shader;
		std::unique_ptr < Texture > m_Texture_Fiona;
		std::unique_ptr < Texture > m_Texture_Tifa;
		std::unique_ptr < VertexBuffer > m_VertexBuffer;

		glm::mat4 m_Proj, m_View;
		VertexBufferLayout m_Layout;

		glm::vec3 m_Translation;

		float m_QuadPosition[2] = { 1000.0f,0.0f };
	};
}
