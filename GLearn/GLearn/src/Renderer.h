#pragma once

#include <include\GLEW\glew.h>


//���Ե��� ΢���ṩ��__debugbreak()
#define ASSERT(x)  if(!(x)) __debugbreak(); 
//#x��������ת��Ϊ�ַ���
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//�˴�ֻ�����GL�������ǲ�����Ҫ���ش�����
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);