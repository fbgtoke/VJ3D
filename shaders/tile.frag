#version 130

uniform sampler2D tex;
uniform vec2 texoffset;

uniform vec3 lightDir;
uniform float ambientColor;

in vec3 normalFrag;
in vec2 texcoordFrag;

out vec4 outColor;

void main() {
	float d = max(0.0, dot(-lightDir, normalize(normalFrag)));
	
  vec2 realtexcoord = texcoordFrag + texoffset;
  vec4 texcolor = min(1.0, d + ambientColor) * texture(tex, realtexcoord);

	outColor = texcolor;
}
