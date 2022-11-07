
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos; //? v3?v4? 都可以,最后都得是v4

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec3 u_color;
out vec3 v_Position;
out vec3 v_color;

void main() //a? 竟然是void main
{
	v_Position = a_Pos;
	v_color = u_color;
	gl_Position = u_ViewProjection *u_Transform*vec4(a_Pos, 1.0); //gl_Position GL默认屏幕空间位置参数	

}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Position; //进来的v_Positon自动插值了???
in vec3 v_color;

void main()
{
	color = vec4(v_color,1.0f);
}


