uniform		sampler2D	tex0;
varying		lowp  vec4	colorVar;
varying		lowp  vec4	colorVar2;
varying		highp vec2	uv0Var;

void main()
{
	vec4 font = texture2D(tex0, uv0Var);
	float alpha = dot(colorVar2.xy, vec2(font.r, font.a));
	gl_FragColor.rgb = colorVar.rgb;
	gl_FragColor.a = colorVar.a * alpha;
}
