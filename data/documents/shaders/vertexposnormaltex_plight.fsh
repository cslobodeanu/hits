#ifndef NUM_POINT_LIGHTS
#define NUM_POINT_LIGHTS 8
#endif

uniform		sampler2D	tex0;
uniform		sampler2D	tex5; // shadow map

uniform 	lowp vec3 	ambientLightColor;
uniform 	lowp vec3 	dirLightColor;
uniform 	highp vec3 	dirLightDirWS;

uniform 	highp vec3 	pLightsPosWS[NUM_POINT_LIGHTS];
uniform 	highp vec3 	pLightsColor[NUM_POINT_LIGHTS];
uniform 	highp vec3 	pLightsAtt[NUM_POINT_LIGHTS];

uniform 	highp vec3 	camDir;
uniform 	highp float shininess;

uniform 	lowp vec4 	globalColor;
uniform 	lowp vec4 	specularColor;
uniform 	lowp vec4 	emissiveColor;

uniform 	highp vec2 	poissonDisk[4];

varying		highp vec2	uv0Var;
varying 	highp vec3	normalVar;
varying 	highp vec3	posVar;

varying		highp vec4	shadowCoord[3];

varying		highp float depth;

uniform		highp float	splitPlanes[3];
uniform		highp vec3	splitViewports[3];

lowp float lambert(highp vec3 lDir, highp vec3 nrm)
{
	return max(0.0, dot(lDir, nrm));// * 0.5 + 0.5;
}

highp float pow8(highp float v) { v *= v; v *= v; v *= v; return v; }

int shadowSplit;

const float divs[3] = float[](7200.0, 10000.0, 14000.0);
const float bss[3] = float[](0.0005, 0.0002, 0.00005);

lowp float ComputeShadow(highp vec3 lDir, highp vec3 nrm)
{	
	lowp float cosTheta = clamp(dot(-lDir, nrm), 0.0, 1.0);
	lowp float bias = bss[shadowSplit]*tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.01);
	
	vec2 shCoord = shadowCoord[shadowSplit].xy;
	
	shCoord.x *= splitViewports[shadowSplit].z;
	shCoord.y *= splitViewports[shadowSplit].z * 3.0 / 2.0;
	
	shCoord += splitViewports[shadowSplit].xy;
		
	lowp float visibility = 1.0;
	
	for(int i = 0; i < 4; i++)
	{
		lowp float sh = texture2D( tex5, shCoord + poissonDisk[i] / divs[shadowSplit] ).z;
		visibility -= step(sh, shadowCoord[shadowSplit].z - bias) * 0.25;
	}
	
	return visibility;
}

vec3 ComputeNormal()
{
	return normalize(normalVar);
}

void main()
{
	shadowSplit = 0;
	if(depth < splitPlanes[0])
		shadowSplit = 0;
	else if(depth < splitPlanes[1])
		shadowSplit = 1;
	else if(depth < splitPlanes[2])
		shadowSplit = 2;

	highp vec3 nrm = ComputeNormal();
	
	lowp float shadow = ComputeShadow(dirLightDirWS, nrm);
	
	highp vec4 color;
	color.rgb = ambientLightColor + mix(vec3(0.0), dirLightColor * lambert(-dirLightDirWS, nrm), shadow);
	color.a = 1.0;
	
	highp vec3 lDir;
	highp vec3 halfDir;
	highp float dist, att, spec;
	
	spec = clamp(dot(normalize(camDir + dirLightDirWS), nrm), 0.0, 1.0);
	spec = pow(spec, shininess);
	
	highp vec3 specular = dirLightColor * mix(0.0, spec, shadow);
	
	/*for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		lDir = (pLightsPosWS[i] - posVar);
		dist = length(lDir);
		lDir = normalize(lDir);
		halfDir = normalize(camDir - lDir);
		att = 1.0 / (pLightsAtt[i].x + pLightsAtt[i].y * dist + pLightsAtt[i].z * dist * dist);
		color.rgb += pLightsColor[i] * lambert(lDir, nrm) * att;
		
		spec = max(0.0, dot(halfDir, nrm));
		spec = pow(spec, shininess);
		specular += pLightsColor[i] * spec * att;
	}*/
	
	//color.rgb = mix(ambientLightColor, color.rgb, shadow);	
	//specular.rgb = mix(vec3(0.0), specular.rgb, shadow);

	lowp vec4 texC = texture2D(tex0, uv0Var);
	gl_FragColor = texC * color * globalColor + specularColor * vec4(specular, 1.0) + emissiveColor;
	
	/*vec4 splitCol = vec4(0.0);
	if(shadowSplit == 0)
		splitCol = vec4(1.0, 0.6, 0.6, 1.0);
	else if(shadowSplit == 1)
		splitCol = vec4(0.6, 1.0, 0.6, 1.0);
	else if(shadowSplit == 2)
		splitCol = vec4(0.6, 0.6, 1.0, 1.0);
		
	gl_FragColor  *= splitCol;*/
}
