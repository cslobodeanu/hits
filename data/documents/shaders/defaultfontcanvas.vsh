attribute	highp vec4	vertex_pos;
attribute	lowp  vec4	vertex_color;
attribute	lowp  vec4	vertex_color2;
attribute	highp vec2	vertex_uv0;

uniform		highp mat4	pvwMat;

varying		lowp  vec4	colorVar;
varying		lowp  vec4	colorVar2;

varying		highp vec2	uv0Var;

void main()
{
	colorVar = vertex_color;
	colorVar2 = vertex_color2;
	uv0Var = vertex_uv0;
	gl_Position = pvwMat * vertex_pos;
}
