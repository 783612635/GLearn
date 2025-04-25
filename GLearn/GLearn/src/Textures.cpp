#include "Textures.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	:m_FilePath(filePath)
	,m_RendererID(0)
	,m_LocalBuffer(nullptr)
	,m_BPP(0),m_Width(0),m_Height(0)
{
	//��ֱ��ת������ΪOpenGLϣ�����ǵ��������ش����½ǿ�ʼ
	//openGL�涨���½�Ϊ(0,0)
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1,&m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//ָ����С���Ŵ��������ֵ������Ҫ��Ⱦ����������С��С�������ʱ����Ҫ��С��Ŵ�
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//����ˮƽ��ֱǶ�뻷�ƣ����ǲ���Ҫ��������Ⱦ����
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0/*���Ǹ�������*/, GL_RGBA8/*�ڲ���ʽ*/, m_Width, m_Height, 0/*����߿�*/, GL_RGBA,GL_UNSIGNED_BYTE,m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	//�������������͵�ͬ�ڽ��������ݴ�CPU���Ƶ�GPU��
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
