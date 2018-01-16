varying	lowp vec4	colorVar;

void main()
{
	gl_FragColor = colorVar * globalColor;
}