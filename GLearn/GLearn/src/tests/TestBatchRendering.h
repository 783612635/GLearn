#pragma once

#include <memory>

#include "Test.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Textures.h"

namespace test
{
	class TestBatchRendering :public Test
	{
	public:
		TestBatchRendering();
		~TestBatchRendering();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr < VertexArray > m_VAO;
		std::unique_ptr < IndexBuffer > m_IndexBuffer;
		std::unique_ptr < Shader > m_Shader;
		std::unique_ptr < Texture > m_Texture;
		std::unique_ptr < VertexBuffer > m_VertexBuffer;

		glm::mat4 m_Proj, m_View;
		VertexBufferLayout m_Layout;

		glm::vec3 m_Translation;
		glm::vec4 m_RGB;
	};
}