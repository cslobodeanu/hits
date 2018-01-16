attribute	highp vec4	vertex_pos;
attribute	highp  vec3	vertex_normal;
attribute	highp vec2	vertex_uv0;

uniform		highp mat4	pvwMat;
uniform		highp mat4	worldMat;
uniform		highp mat4	viewMat;
uniform		highp mat4	shadowMat[3];

varying		highp vec2	uv0Var;
varying 	highp vec3	normalVar;
varying 	highp vec3	posVar;

varying		highp vec4	shadowCoord[3];

varying 	highp float depth;

void main()
{
	normalVar = (worldMat * vec4(vertex_normal, 0.0)).xyz;
	posVar = (worldMat * vertex_pos).xyz;
	for(int i = 0; i < 3; i++)
		shadowCoord[i] = shadowMat[i] * vec4(posVar, 1.0);
		
	uv0Var = vertex_uv0;
	depth = -(viewMat * vec4(posVar, 1.0)).z;
	gl_Position = pvwMat * vec4(posVar, 1.0);
}

// #tags: shadow mapping, directional lights
