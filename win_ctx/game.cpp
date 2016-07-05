#include <iostream>
#include <vector>
#include "game.h"
#include "ogl.h"

std::vector<float> square(const float x, const float y, const float w, const float h) {
	
	std::vector<float> verts = {
		x,			y,
		(x - w),	y,
		x,			(y - h),
		x,			(y - h),
		(x - w),	(y),
		(x - w),	(y - h)
	};
	
	return verts;
}

namespace game {
	
	GLuint shader_programme; 
	GLuint vao;
	
	void game::init() {

		std::cout << "Starting game loop" << std::endl;

		std::vector<float> vss = square(0.0f, 0.0f, 0.4f, 0.4f);

		float* points = &vss[0];

		GLuint vbo = 0;
		
		// Insert n number of unused names to vbo
		glGenBuffers(1, &vbo);
		// Acquire buffer state (alternatively glCreateBuffer() to do both)
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// Allocate mutable data store
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

		vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		const char* vertex_shader =
			"#version 450\n"
			"in vec2 vp;"
			"void main () {"
			"  gl_Position = vec4 (vp, 0.0, 1.0);"
			"}";

		const char* fragment_shader =
			"#version 450\n"
			"out vec4 frag_colour;"
			"void main () {"
			"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
			"}";

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader, NULL);
		glCompileShader(fs);

		shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);
	}

	void game::update(int dt) {
		// wipe the drawing surface clear
		glClearColor(255, 255, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}