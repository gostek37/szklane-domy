#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aOffset;
layout (location = 3) in vec3 aColour;

out vec3 vertColour;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 offsetModel = model;
	offsetModel[3].xyz += aOffset;
										// the last value is the w component (x, y, z, w)
	gl_Position = projection * view * offsetModel * vec4(aPos, 1.0);
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertColour = aColour;
	TexCoords = aTexCoord;
}