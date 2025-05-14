#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data,unsigned int size);
	//������Ϊ�Ƕ�̬�������ݣ����ǿ�����Ԥ��һ���������ɴ�С��Ȼ������ٸ���ʵ���Զ����䡣
	//����������С֮ǰ������Ҫ���ǵ��ǣ���������������ɶ��ٶ��㣬ÿ���������ж�����������
	VertexBuffer(unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_RendererID;
};