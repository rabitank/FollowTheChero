
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos; //? v3?v4? 都可以,最后都得是v4

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() //a? 竟然是void main
{
	gl_Position = u_ViewProjection *u_Transform*vec4(a_Pos, 1.0); //gl_Position GL默认屏幕空间位置参数	

}

#type fragment
#version 330 core

uniform vec4 u_color;
layout(location = 0) out vec4 color;

void main()
{
	color = u_color;
}


