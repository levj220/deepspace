#version 400

in vec2 ex_TexCoord;
out vec4 out_Color;

uniform sampler2D baseImage;
uniform sampler2D displacementMap;

void main(void)
{
	//out_Color = texture2D(baseImage, ex_TexCoord.st) * (0.6 + texture2D(displacementMap, ex_TexCoord.st));
	vec4 texSample = texture2D(displacementMap, ex_TexCoord.st);
	float perc = (texSample.x + texSample.y + texSample.z)/3.0;
	out_Color = vec4(perc, 1.0 - perc, 0.0, 1.0);
}