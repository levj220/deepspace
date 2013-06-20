#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_TexCoord;


out vec2 ex_TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform sampler2D displacementMap;

void main(void)
{
	vec4 newPos;
	vec4 dv;
	float df;
	
	ex_TexCoord = in_TexCoord;
	
	dv = texture2D(displacementMap, in_TexCoord.xy);
	df = (dv.x + dv.y + dv.z)/3.0; 
	newPos = vec4(vec3(0.0, 1.0, 0.0) * df * 60.0, 0.0) + in_Position;
	
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * newPos;
}