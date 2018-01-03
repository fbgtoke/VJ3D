#version 330 core

uniform sampler2D tex;
uniform vec3 fontcolor;

in vec2 texcoordFrag;
out vec4 color;

void main() {
  vec4 texcolor = texture(tex, texcoordFrag);

  if (texcolor.w < 0.25)
    discard;

  color = vec4(fontcolor, 1);
}
