#pragma once

#include <include\GLEW\glew.h>


//断言调用 微软提供的__debugbreak()
#define ASSERT(x)  if(!(x)) __debugbreak(); 
//#x将函数名转换为字符串
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//此处只是清除GL错误，我们并不需要返回错误码
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);