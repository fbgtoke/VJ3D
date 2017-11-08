#version 130

uniform mat4 PM, VM;
uniform mat4 modelview;
uniform vec2 texCoordDispl;

in vec2 position;
in vec2 texCoord;
out vec2 texCoordFrag;

void main()
{
	// Pass texture coordinates to access a given texture atlas
	texCoordFrag = texCoord + texCoordDispl;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = PM * VM * modelview * vec4(position, 0.0, 1.0);
}

