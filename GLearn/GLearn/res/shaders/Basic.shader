#shader vertex      
#version 330 core
        
layout(location = 0) in/*in 代表输入，相对的out代表输出*/ vec4 position;
layout(location = 1) in vec2 texCoord;
        
out vec2 v_TexCoord;

void main()
{
    gl_Position = position;
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
    //顶点着色器和片段着色器之间数据通过一种叫varying的方式来传递
    //在OpenGL中，这就是一种输入输出系统
    vec4 texColor = texture(u_Texture,v_TexCoord);
    color = texColor;
};