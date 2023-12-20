#version 330

uniform mat4 transformation;
uniform vec3 color;

layout(location = 0) in vec3 position;

out vec3 vertColor;

void main() {
	gl_Position = transformation * vec4(position, 1.0);
	vertColor = color;
}