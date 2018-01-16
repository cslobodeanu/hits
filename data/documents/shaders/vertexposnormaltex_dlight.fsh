uniform		sampler2D	tex0;

uniform 	lowp vec3 	ambientLightColor;
uniform 	lowp vec3 	dirLightColor;
uniform 	highp vec3 	dirLightDirWS;

uniform 	lowp vec4 	globalColor;

varying		highp vec2	uv0Var;
varying 	highp vec3	normalVar;

lowp float lambert(highp vec3 lDir, highp vec3 nrm)
{
	//return max(0.0, dot(lDir, nrm)) * 0.5 + 0.5; // Valve style
	return max(0.0, dot(lDir, nrm));
}

void main()
{
	lowp vec4 color;
	color.rgb = ambientLightColor + dirLightColor * lambert(-dirLightDirWS, normalVar);
	color.a = 1.0;

	gl_FragColor = texture2D(tex0, uv0Var) * color * globalColor;
}
