#shader vertex
#version 330 core
//记得检查语法!!!
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textCoord; //for uv coordnate
//vec4:opengl中位置就是一个vec4,没办法,后两个直接补零了. 
//loaction 对应属性索引 position:为属性命的名

uniform mat4 u_MVP;

out vec2 v_TexCoord; //in out 标记
//为了顶点数据以及不同着色器之间的通信(使用叫varying的东西)
//输出纹理坐标
void main()
{
	gl_Position = u_MVP*position;  //gl_Position大概率自带的屏幕空间
	v_TexCoord = textCoord;
}
; //完成,强行赋值

#shader fragment
#version 330 core  //使用 glsl(opengl语言)330version ,core:不允许使用已弃用方法

in vec2 v_TexCoord; //读入 v_TexCoord
//统一变量

uniform sampler2D u_Texture;//取样插槽
uniform vec4 u_Color; //由于没有用到,实际上,编译后它会被删除以至于你找不到

layout(location = 0) out vec4 color;   //vec4:opengl中颜色也是一个vec4. 
void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord); 
	color = texColor;  //指定颜色 rgba
}
; //完成