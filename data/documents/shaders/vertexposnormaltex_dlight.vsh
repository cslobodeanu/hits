attribute	highp vec4	vertex_pos;
attribute	highp  vec3	vertex_normal;
attribute	highp vec2	vertex_uv0;

uniform		highp mat4	pvwMat;
uniform		highp mat4	worldMat;

varying		highp vec2	uv0Var;
varying		lowp vec4	colorVar;
varying 	highp vec3	normalVar;

void main()
{
	highp vec4 nrm = worldMat * vec4(vertex_normal, 0.0);
	normalVar = normalize(nrm.xyz);	
	uv0Var = vertex_uv0;
	gl_Position = pvwMat * worldMat * vertex_pos;
}

// #tags: directional lights