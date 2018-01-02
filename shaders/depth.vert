#version 330 core

uniform mat4 PM, VM, TG;

in vec3 vertex;

void main() {
  gl_Position = PM * VM * TG * vec4(vertex, 1);
}