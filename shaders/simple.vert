#version 330 core

uniform mat4 MVP;

in vec3 vertex;
in vec3 normal;
in vec2 texcoord;

out vec3 normalFrag;
out vec2 texcoordFrag;

void main() {
	normalFrag = normal;
	texcoordFrag = texcoord;
	
	gl_Position = MVP * vec4(vertex, 1.0);
}
