attribute	highp vec4	vertex_pos;
attribute	lowp  vec4	vertex_color;
attribute	highp vec2	vertex_uv0;

uniform		highp mat4	pvwMat;
uniform		highp mat4	worldMat;
uniform 	lowp vec4 	globalColor;

varying		lowp  vec4	colorVar;
varying		highp vec2	uv0Var;

void main()
{
	colorVar = vertex_color * globalColor;
	uv0Var = vertex_uv0;
	gl_Position = pvwMat * worldMat * vertex_pos;
}
