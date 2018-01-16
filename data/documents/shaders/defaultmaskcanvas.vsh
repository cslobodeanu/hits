attribute	highp vec4	vertex_pos;
attribute	lowp  vec4	vertex_color;
attribute	highp vec2	vertex_uv0;
attribute	highp vec2	vertex_uv1;

uniform		highp mat4	pvwMat;

varying		lowp  vec4	colorVar;
varying		highp vec2	uv0Var;
varying		highp vec2	uv1Var;

void main()
{
	colorVar = vertex_color;
	uv0Var = vertex_uv0;
	uv1Var = vertex_uv1;
	gl_Position = pvwMat * vertex_pos;
}
