#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data,unsigned int size);
	//这里因为是动态顶点数据，我们可以先预设一个最大的容纳大小，然后后面再根据实际自动分配。
	//在设置最大大小之前，我们要考虑的是，我们期望最大容纳多少顶点，每个顶点里有多少描述数据
	VertexBuffer(unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_RendererID;
};