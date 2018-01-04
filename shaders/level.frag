#version 330 core

uniform sampler2D tex;
uniform sampler2D shadow;

uniform vec2 texoffset;

uniform vec3 lightDir;
uniform vec3 ambientColor;

in vec3 normalFrag;
in vec2 texcoordFrag;
in vec4 shadowCoord;

layout(location = 0) out vec4 outColor;

void main() {
  float cosTheta = clamp(dot(normalFrag, -lightDir), 0, 1);
	vec4 texcolor = texture(tex, texcoordFrag + texoffset);

  float bias = 0.005;
  float visibility = 1.0;
  if (texture(shadow, shadowCoord.xy).z < shadowCoord.z - bias) {
    visibility = 0.5;
  }

  vec4 color = 
    vec4(ambientColor, 0) + 
    visibility * texcolor;

	outColor = color;
}
