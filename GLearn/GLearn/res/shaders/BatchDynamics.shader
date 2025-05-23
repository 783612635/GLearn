#shader vertex      
#version 330 core
        
layout(location = 0) in/*in 代表输入，相对的out代表输出*/ vec4 position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
   
uniform mat4 u_MVP;//model view projection matrix

void main()
{
    gl_Position = u_MVP * position;
     v_TexCoord = texCoord;
     v_TexIndex = a_TexIndex;
     v_Color = a_Color;
};

 #shader fragment
 #version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec4 v_Color;

uniform sampler2D u_Textures[2];
        
void main()
{
    int index = int(v_TexIndex);
    vec4 texColor = texture(u_Textures[index],v_TexCoord);
    color = texColor;
    //color = vec4(v_TexIndex,v_TexIndex,v_TexIndex,1.0);
};