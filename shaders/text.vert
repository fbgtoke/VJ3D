#version 130

uniform mat4 PM, VM;
uniform mat4 modelview;

in vec2 position;
in vec2 texCoord;
out vec2 texCoordFrag;

void main()
{
	// Pass texture coordinates
	texCoordFrag = texCoord;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = PM * VM * modelview * vec4(position, 0.0, 1.0);
}

