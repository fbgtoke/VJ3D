#version 330 core

uniform mat4 MVP;

in vec3 vertex;

void main() {
  gl_Position = MVP * vec4(vertex, 1);
}