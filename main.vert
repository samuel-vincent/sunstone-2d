#version 330

in vec2 vp;

uniform mat4 MVP;

void main () {
	gl_Position = vec4 (vp, 0.0, 1.0);
}