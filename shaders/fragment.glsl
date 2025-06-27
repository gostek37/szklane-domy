#version 330 core

out vec4 FragColor;

in vec3 vertColour;
in vec2 TexCoords;

uniform sampler2D texture1;

void main() {
	vec4 outColour = texture(texture1, TexCoords) * vec4(vertColour, 1.0);
	if (outColour.a < 0.01) {
		discard;
	}
	// Not dealing with transparency for now
	FragColor = vec4(outColour.rgb, 1.0);
}