#version 330 core

uniform sampler2D tex;

in vec2 texcoordFrag;
out vec4 color;

void main() {
  vec3 texcolor = texture(tex, texcoordFrag).rgb;
  float gray = dot(texcolor, vec3(0.299, 0.587, 0.114));
  color = vec4(gray, gray, gray, 1);
}
