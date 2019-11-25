#version 450 core

in vec4 exCoords;
in vec3 exNormal;
in vec2 exUV;

in float exTime;
float scatterCoef = 0.20f;
float gradientCheck = 1.0f - scatterCoef;

out vec4 FragColor;

void main(void)
{
	vec4 baseColor = vec4(0.7f, 0.94f, 1.0f, 0.5f) + scatterCoef * cos(exTime - exCoords.y);
	if (exCoords.y < 0.075f) {
		discard;
	}

	if (baseColor.b < gradientCheck) {
		discard;
	}

	FragColor = baseColor;
}