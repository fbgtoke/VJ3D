#version 330 core

uniform sampler2D tex;

uniform vec2 texoffset;

uniform vec3 lightDir;
uniform vec3 ambientColor;

in vec3 normalFrag;
in vec2 texcoordFrag;

layout(location = 0) out vec4 outColor;

void main() {
  float cosTheta = clamp(dot(normalFrag, -lightDir), 0, 1);
  vec4 texcolor = texture(tex, texcoordFrag + texoffset);

  vec4 color = texcolor * cosTheta;

	outColor = color;
}
