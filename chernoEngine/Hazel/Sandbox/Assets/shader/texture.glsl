#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos; 
layout(location = 1) in vec4 a_Color; 
layout(location = 2) in vec2 a_UVCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec2 v_UVCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main() 
{
	v_UVCoord = a_UVCoord;
	v_Color = a_Color;	
	v_TexIndex= a_TexIndex;	
	v_TilingFactor= a_TilingFactor;	
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0); 

}

#type fragment
#version 330 core

uniform sampler2D u_Texture[32]; //2D纹理采样类型,int :slot [32] 支持32个槽位


in vec2 v_UVCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

layout(location = 0) out vec4 color;

void main()
{
	
	color = texture(u_Texture[int(v_TexIndex)], v_UVCoord*v_TilingFactor)*v_Color;
	//color= vec4(v_TexIndex/32,0.0,0.0,1.0);
	//color= vec4(v_UVCoord,0.0,1.0);
	//color= v_Color;


}


