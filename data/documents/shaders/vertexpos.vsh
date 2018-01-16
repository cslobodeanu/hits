attribute	highp vec4	vertex_pos;

uniform		highp mat4	pvwMat;

void main()
{
	gl_Position = pvwMat * vertex_pos;
}
