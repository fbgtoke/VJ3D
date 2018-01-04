#version 330 core

uniform sampler2D tex;
uniform sampler2D shadow;

uniform vec3 lightDir;
uniform vec3 lightColor;

in vec3 normalFrag;
in vec2 texcoordFrag;
in vec4 shadowCoord;
in vec4 vertexWorld;

layout(location = 0) out vec4 outColor;

void main() {
  float cosTheta = clamp(dot(normalFrag, -lightDir), 0, 1);
  vec4 texcolor = texture(tex, texcoordFrag);

  float bias = 0.005;
  float visibility = 1.0;

  if (texture(shadow, shadowCoord.xy).r < shadowCoord.z - bias)
  {
    visibility = 0.5;
  }

  vec4 color =
    visibility * texcolor * cosTheta * vec4(lightColor, 1);
  
  outColor = color;
}
