#version 330 core

uniform sampler2D tex;
uniform vec2 texoffset;

uniform vec3 lightDir;
uniform float ambientColor;
uniform float diffColor;

in vec3 normalFrag;
in vec2 texcoordFrag;

layout(location = 0) out vec4 outColor;

void main() {
	float d = max(0.0, dot(-lightDir, normalize(normalFrag)));
	
  vec2 realtexcoord = texcoordFrag + texoffset;
  vec4 texcolor = texture(tex, realtexcoord);
	vec4 color = min(1.0, d + diffColor + ambientColor) * texcolor;

	outColor = color;
}
