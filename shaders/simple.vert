#version 330 core

uniform mat4 PM, VM, TG;

uniform mat3 rotation;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec3 normalFrag;
out vec2 texcoordFrag;

void main() {
	normalFrag = normalize(rotation * normal);
	texcoordFrag = texcoord;
	
	gl_Position = PM * VM * TG * vec4(vertex, 1.0);
}
