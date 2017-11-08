#version 130

uniform vec3 lightDir;
uniform float ambientColor;
uniform bool enableLight;

in vec3 fragNormal;
in vec4 fragColor;

out vec4 outColor;

void main() {
	if (enableLight) {
		float d = max(0.0, dot(-lightDir, normalize(fragNormal)));
		outColor = min(1.0, d + ambientColor) * fragColor;
	} else {
		outColor = fragColor;
	}
}
