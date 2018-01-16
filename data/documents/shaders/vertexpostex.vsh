attribute	highp vec4	vertex_pos;
attribute	highp vec2	vertex_uv0;

uniform		highp mat4	pvwMat;
uniform		highp mat4	worldMat;
varying		highp vec2	uv0Var;

void main()
{
	uv0Var = vertex_uv0;
	gl_Position = pvwMat * worldMat * vertex_pos;
}
