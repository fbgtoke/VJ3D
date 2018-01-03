#version 330 core

uniform mat4 PM, VM, TG;

in vec3 vertex;
in vec2 texcoord;

out vec2 texcoordFrag;

void main() {
	texcoordFrag = texcoord;
	
	gl_Position = PM * VM * TG * vec4(vertex, 1.0);
}
