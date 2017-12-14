#version 130

uniform mat4 PM, VM, TG;

in vec3 vertex;
in vec3 normal;
in vec2 texcoord;

out vec3 normalFrag;
out vec2 texcoordFrag;

void main() {
	normalFrag = normal;
  texcoordFrag = texcoord;
	
	gl_Position = PM * VM * TG * vec4(vertex, 1.0);
}
