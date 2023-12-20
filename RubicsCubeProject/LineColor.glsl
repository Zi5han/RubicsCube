#version 330

uniform mat4 color;

out vec3 vertColor;

void main() {
	vertColor = color;
}