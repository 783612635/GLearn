#include "Textures.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	:m_FilePath(filePath)
	,m_RendererID(0)
	,m_LocalBuffer(nullptr)
	,m_BPP(0),m_Width(0),m_Height(0)
{
	//垂直翻转纹理，因为OpenGL希望我们的纹理像素从左下角开始
	//openGL规定左下角为(0,0)
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1,&m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//指定缩小、放大过滤器，值得是需要渲染区域比纹理大小更小、或更大时，需要缩小或放大
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//设置水平或垂直嵌入环绕，我们不需要它扩大渲染区域
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0/*不是复合纹理*/, GL_RGBA8/*内部格式*/, m_Width, m_Height, 0/*无需边框*/, GL_RGBA,GL_UNSIGNED_BYTE,m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	//上述方法基本就等同于将纹理数据从CPU复制到GPU上
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
