#shader vertex      
#version 330 core
        
layout(location = 0) in/*in 代表输入，相对的out代表输出*/ vec4 position;
layout(location = 1) in/*in 代表输入，相对的out代表输出*/ vec4 a_Color;

out vec4 v_Color;
   
uniform mat4 u_MVP;//model view projection matrix

void main()
{
    v_Color =  a_Color;
    gl_Position = u_MVP * position;
};

 #shader fragment
 #version 330 core
        
layout(location = 0) out vec4 color;

in vec4 v_Color;

uniform vec4 u_Color;
        
void main()
{
    color = v_Color;
};