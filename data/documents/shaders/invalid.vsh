attribute	highp	vec4	vertex_pos;
uniform		highp	mat4	pvwMat;
uniform		highp	mat4	worldMat;
void main()
{
	gl_Position = pvwMat * worldMat * vertex_pos;
}