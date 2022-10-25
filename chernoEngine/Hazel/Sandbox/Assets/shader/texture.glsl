#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos; //? v3?v4? 都可以,最后都得是v4
layout(location = 1) in vec2 a_UVCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec2 v_UVCoord;

void main() //a? 竟然是void main
{
	v_UVCoord = a_UVCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0); //gl_Position GL默认屏幕空间位置参数	

}

#type fragment
#version 330 core

uniform sampler2D u_Texture; //2D纹理采样类型,int :slot

layout(location = 0) out vec4 color;
in vec2 v_UVCoord;

void main()
{
	color = texture(u_Texture, v_UVCoord);
}


