#shader vertex      
#version 330 core
        
layout(location = 0) in/*in �������룬��Ե�out�������*/ vec4 position;
layout(location = 1) in vec2 texCoord;
        
out vec2 v_TexCoord;

uniform mat4 u_MVP;//model view projection matrix

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
};

 #shader fragment
 #version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
        
void main()
{
    //������ɫ����Ƭ����ɫ��֮������ͨ��һ�ֽ�varying�ķ�ʽ������
    //��OpenGL�У������һ���������ϵͳ
    vec4 texColor = texture(u_Texture,v_TexCoord);
    color = texColor;
};