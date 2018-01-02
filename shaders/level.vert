#version 330 core

uniform mat4 MVP, depthMVP;

in vec3 vertex;
in vec3 normal;
in vec2 texcoord;

out vec3 normalFrag;
out vec2 texcoordFrag;
out vec4 shadowCoord;

void main() {
	normalFrag = normal;
	texcoordFrag = texcoord;
	
	gl_Position = MVP * vec4(vertex, 1.0);
  shadowCoord = depthMVP * vec4(vertex, 1.0);
}
