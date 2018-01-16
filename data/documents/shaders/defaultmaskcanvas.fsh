uniform		sampler2D	tex0;
uniform		sampler2D	tex1;
varying		lowp  vec4	colorVar;
varying		highp vec2	uv0Var;
varying		highp vec2	uv1Var;

uniform 	lowp vec4 	maskColor;

void main()
{
	vec3 mask = mix(vec3(1.0), texture2D(tex1, uv1Var).rgb * maskColor.rgb, maskColor.a);
	vec4 result = texture2D(tex0, uv0Var) * colorVar;
	result.rgb *= mask.rgb;
	gl_FragColor = result;
}
