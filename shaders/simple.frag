#version 130

uniform vec4 color;
uniform sampler2D tex;

uniform vec3 lightDir;
uniform float ambientColor;

in vec3 normalFrag;
in vec2 texcoordFrag;

out vec4 outColor;

void main() {
	float d = max(0.0, dot(-lightDir, normalize(normalFrag)));
	
	vec4 texcolor = min(1.0, d + ambientColor) * texture(tex, texcoordFrag);

	outColor = color * texcolor;
}
