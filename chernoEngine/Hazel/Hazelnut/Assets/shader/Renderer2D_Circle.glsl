// Renderer2D Circle Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera //uniform Buffer Camera is the block name?
{
	mat4 u_ViewProjection;
};



struct VertexOutput
{
	vec3 LoaclPosition;
	float Thickness;
	vec4 Color;
	float Fade; // 36B > 16*2 : 0,1,2,3 layer
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.LoaclPosition= a_LocalPosition;
	Output.Thickness= a_Thickness;
	Output.Color = a_Color;
	Output.Fade= a_Fade;
	v_EntityID = a_EntityID;
	
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 LoaclPosition;
	float Thickness;
	vec4 Color;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
//flat ¹Ø±Õ²åÖµ
layout (location = 4) in flat int v_EntityID;

void main()
{
    //LocalPosition-> UV ,LocalPosition is based on WorldPosition,it is interpreted 
    float distance = 1.0 - length(Input.LoaclPosition.xy);
	float circle = smoothstep(0.0,Input.Fade,distance);
	circle*=smoothstep(Input.Thickness+Input.Fade,Input.Thickness,distance); //smoothStep can change the step direction
     
	if(circle==0.0)
		discard;	

	o_Color= Input.Color;
	o_Color.a *= circle;
	o_EntityID = v_EntityID;


}
