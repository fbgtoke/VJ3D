#version 330 core

uniform mat4 PM;
uniform mat4 VM;
uniform mat4 TG;

uniform mat3 rotation;

uniform mat4 depthPM;
uniform mat4 depthVM;
uniform mat4 biasDepthMatrix;

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

out vec3 normalFrag;
out vec2 texcoordFrag;
out vec4 shadowCoord;

void main() {
	normalFrag = normalize(rotation * normal);
	texcoordFrag = texcoord;

  gl_Position = PM * VM * TG * vec4(vertex, 1.0);
  shadowCoord = (biasDepthMatrix * depthPM * depthVM * TG) * vec4(vertex, 1.0);
}
