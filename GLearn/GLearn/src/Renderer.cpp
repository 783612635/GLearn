#include "Renderer.h"

#include <iostream>

void GLClearError(){
    while (glGetError() != GL_NO_ERROR);//while (!glGetError());GL_NO_ERRORΪ0
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error](" << error << ");" << function
            << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const size_t ibCount, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibCount, GL_UNSIGNED_INT, nullptr));
}
