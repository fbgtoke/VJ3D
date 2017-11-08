#version 130

uniform mat4 PM, VM;
uniform mat4 modelMatrix;
uniform vec4 color;

in vec3 position;
in vec3 normal;

out vec3 fragNormal;
out vec4 fragColor;

void main() {
	fragNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
	fragColor = color;

	gl_Position = PM * VM * modelMatrix * vec4(position, 1.0);
}
