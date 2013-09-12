#pragma once

#include "headers.h"

class cCube { // (-1, -1, -1) to (1, 1, 1)
private:
	GLuint vertex_buffer, index_buffer;
	GLuint vertex_array;
	GLuint vertex_shader, fragment_shader;
	GLuint shader_program;
	GLint um4model, um4view, um4projection;
	GLint uv3color;
public:
	cCube() {
		const vec3 c[] = { // corners of a cube
			{1, 1, 1},
			{1, 1, -1},
			{1, -1, -1},
			{1, -1, 1},
			{-1, 1, 1},
			{-1, 1, -1},
			{-1, -1, -1},
			{-1, -1, 1}
		};

		const vec3 vertices[] = { // 6 faces of a cube
			c[0], c[1], c[2], c[3], // +x
			c[5], c[4], c[7], c[6], // -x
			c[0], c[4], c[5], c[1], // +y
			c[2], c[6], c[7], c[3], // -y
			c[0], c[3], c[7], c[4], // +z
			c[5], c[6], c[2], c[1] // -z
		};
		vertex_buffer = 0; glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		const GLubyte indices[] = {
			0, 1, 2, // +x
			2, 3, 0,
			4, 5, 6, // -x
			6, 7, 4,
			8, 9, 10, // +y
			10, 11, 8,
			12, 13, 14, // -y
			14, 15, 12,
			16, 17, 18, // +z
			18, 19, 16,
			20, 21, 22, // -z
			22, 23, 20
		};
		index_buffer = 0; glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

		const GLchar *vertex_shader_string =
			"#version 400\n"
			"in vec3 vertex_position;"
			"uniform mat4 model_matrix;"
			"uniform mat4 view_matrix;"
			"uniform mat4 projection_matrix;"
			"uniform vec3 solid_color;"
			"out vec3 color;"
			"void main() {"
			"	gl_Position = (projection_matrix * view_matrix * model_matrix) * vec4(vertex_position + vec3(0, 0, gl_InstanceID * 4.0), 1.0);"
			"	color = solid_color;"
			"}";
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_string, NULL);
		glCompileShader(vertex_shader);

		const GLchar *fragment_shader_string =
			"#version 400\n"
			"in vec3 color;"
			"out vec4 frag_color;"
			"void main() {"
			"	frag_color = vec4(color, 1.0);"
			"}";
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_string, NULL);
		glCompileShader(fragment_shader);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		um4model = glGetUniformLocation(shader_program, "model_matrix");
		um4view = glGetUniformLocation(shader_program, "view_matrix");
		um4projection = glGetUniformLocation(shader_program, "projection_matrix");
		uv3color = glGetUniformLocation(shader_program, "solid_color");
	}

	virtual ~cCube() {
		glDetachShader(shader_program, vertex_shader);
		glDetachShader(shader_program, fragment_shader);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &index_buffer);
		glDeleteVertexArrays(1, &vertex_array);
	}

	void render(vec3 translation) {
		mat4 m4model = mat4::translation(translation);
		glUniformMatrix4fv(um4model, 1, GL_FALSE, m4model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
	}

	void preRender() {
		glUseProgram(shader_program);
		vec3 color = {1.0f, 0.8f, 0.5f};
		glUniform3fv(uv3color, 1, color);
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	}

	void setViewMatrix(const GLfloat *view_matrix) const {
		glUniformMatrix4fv(um4view, 1, GL_FALSE, view_matrix);
	}

	void setProjectionMatrix(const GLfloat *projection_matrix) const {
		glUniformMatrix4fv(um4projection, 1, GL_FALSE, projection_matrix);
	}
};

