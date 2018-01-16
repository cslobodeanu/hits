attribute	highp	vec4	vertex_pos;
attribute	lowp	vec4	vertex_color;

uniform		highp	mat4	pvwMat;
uniform		lowp 	vec4 	globalColor;

varying		lowp	vec4	colorVar;

void main()
{
	colorVar = vertex_color * globalColor;
	gl_Position = pvwMat * vertex_pos;
}