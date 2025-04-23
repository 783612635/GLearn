#pragma once

#include <include\GLEW\glew.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x)  if(!(x)) __debugbreak()

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//此处只是清除GL错误，我们并不需要返回错误码
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& vb, const Shader& shader) const;

private:

};