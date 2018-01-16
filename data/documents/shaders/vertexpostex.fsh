uniform		sampler2D	tex0;
uniform 	lowp vec4 	globalColor;
varying		highp vec2	uv0Var;

void main()
{
	gl_FragColor = texture2D(tex0, uv0Var) * globalColor;
}
