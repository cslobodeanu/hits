uniform		sampler2D	tex0;
varying		lowp  vec4	colorVar;
varying		highp vec2	uv0Var;

void main()
{
	gl_FragColor = texture2D(tex0, uv0Var) * colorVar;
}
