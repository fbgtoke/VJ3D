#version 330 core

uniform mat4 PM, VM, TG;
uniform mat4 depthPM, depthVM, biasDepthMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 texcoord;

out vec3 normalFrag;
out vec2 texcoordFrag;
out vec4 shadowCoord;

void main() {
	normalFrag = normal;
	texcoordFrag = texcoord;
	
	gl_Position =
    PM * VM * TG * vec4(vertex, 1.0);
  shadowCoord =
    biasDepthMatrix * depthPM * depthVM * TG * vec4(vertex, 1.0);
}
