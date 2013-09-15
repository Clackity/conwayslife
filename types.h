/* handy types for OpenGL stuff

	@author Atlee Brink

	notes:
		some handy types with built-in functions
		I've tried to keep it so that these can be initialized as you'd expect,
			and that means not having constructors (else the compiler throws a fit).
			I've included some static "make" functions to get around that, check them out.
		
*/

#pragma once

#include "GL/glew.h"
#include <cmath>

////////
// vec3
////////
union vec3 {
	struct { GLfloat x, y, z; };
	struct { GLfloat r, g, b; };
	GLfloat f[3];

	static vec3 make(GLfloat x_r, GLfloat y_g, GLfloat z_b) { vec3 v = { x_r, y_g, z_b }; return v; }
	static vec3 make(GLfloat value) { vec3 v = { value, value, value }; return v; }

	static vec3 cross(const vec3 &left, const vec3 &right) { vec3 v = { left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y }; return v; }
	static GLfloat dot(const vec3 &left, const vec3 &right) { return left.x * right.x + left.y * right.y + left.z * right.z; }
	static vec3 normal(const vec3 &in) {
		GLfloat invl = 1.0f / sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
		vec3 v = { in.x * invl, in.y * invl, in.z * invl };
		return v;
	}

	vec3 cross(const vec3 &rvalue) const { return cross(*this, rvalue); }
	GLfloat dot(const vec3 &rvalue) const { return dot(*this, rvalue); }
	vec3 normal() { return normal(*this); }

	vec3 operator + (const vec3 &right) const { vec3 v = { x + right.x, y + right.y, z + right.z }; return v; }
	vec3 operator + (const GLfloat &right) const { vec3 v = { x + right, y + right, z + right }; return v; }
	vec3 operator - (const vec3 &right) const { vec3 v = { x - right.x, y - right.y, z - right.z }; return v; }
	vec3 operator - (const GLfloat &right) const { vec3 v = { x - right, y - right, z - right }; return v; }
	vec3 operator * (const GLfloat &right) const { vec3 v = { x * right, y * right, z * right }; return v; }

	operator GLfloat* () { return f; }
};

////////
// vec4
////////
union vec4 {
	struct { GLfloat x, y, z, w; };
	struct { GLfloat r, g, b, a; };
	GLfloat f[4];
	vec3 vec3;

	static vec4 make(GLfloat x_r, GLfloat y_g, GLfloat z_b, GLfloat w_a) { vec4 v = { x_r, y_g, z_b, w_a }; return v; }
	static vec4 make(::vec3 vec3, GLfloat w_a) { vec4 v = { vec3.x, vec3.y, vec3.z, w_a }; return v; }

	operator GLfloat* () { return f; }
};

////////
// mat4
////////
union mat4 {
	struct {
		GLfloat a, b, c, d;
		GLfloat e, f, g, h;
		GLfloat i, j, k, l;
		GLfloat m, n, o, p;
	};
	GLfloat fl[16];

	static mat4 identity() { mat4 _identity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; return _identity; }
	static mat4 lookatrh(const vec3 &eye, const vec3 &at, const vec3 &up) {
		vec3 zaxis = vec3::normal(eye - at);
		vec3 xaxis = vec3::normal(vec3::cross(up, zaxis));
		vec3 yaxis = vec3::cross(zaxis, xaxis);
		mat4 _lookatrh = {
			xaxis.x, yaxis.x, zaxis.x, 0,
			xaxis.y, yaxis.y, zaxis.y, 0,
			xaxis.z, yaxis.z, zaxis.z, 0,
			-vec3::dot(xaxis, eye), -vec3::dot(yaxis, eye), -vec3::dot(zaxis, eye), 1
		};
		return _lookatrh;
	}
	static mat4 multiply(const mat4 &left, const mat4 &right) {
		mat4 _product = {
			left.a * right.a + left.b * right.e + left.c * right.i + left.d * right.m,
				left.a * right.b + left.b * right.f + left.c * right.j + left.d * right.n,
					left.a * right.c + left.b * right.g + left.c * right.k + left.d * right.o,
						left.a * right.d + left.b * right.h + left.c * right.l + left.d * right.p,
			left.e * right.a + left.f * right.e + left.g * right.i + left.h * right.m,
				left.e * right.b + left.f * right.f + left.g * right.j + left.h * right.n,
					left.e * right.c + left.f * right.g + left.g * right.k + left.h * right.o,
						left.e * right.d + left.f * right.h + left.g * right.l + left.h * right.p,
			left.i * right.a + left.j * right.e + left.k * right.i + left.l * right.m,
				left.i * right.b + left.j * right.f + left.k * right.j + left.l * right.n,
					left.i * right.c + left.j * right.g + left.k * right.k + left.l * right.o,
						left.i * right.d + left.j * right.h + left.k * right.l + left.l * right.p,
			left.m * right.a + left.n * right.e + left.o * right.i + left.p * right.m,
				left.m * right.b + left.n * right.f + left.o * right.j + left.p * right.n,
					left.m * right.c + left.n * right.g + left.o * right.k + left.p * right.o,
						left.m * right.d + left.n * right.h + left.o * right.l + left.p * right.p

		};
		return _product;
	}
	static mat4 projectionrh(GLfloat near = 1.0f, GLfloat far = 1000.0f, GLfloat fov = 1.221730f, GLfloat aspect = (4.0f / 3.0f)) {
		//GLfloat range = tan(fov * 0.5f) * near;
		//GLfloat sx = (2.0f * near) / (range * aspect + range * aspect);
		//GLfloat sy = near / range;
		//GLfloat sz = -(far + near) / (far - near);
		//GLfloat pz = -(2.0f * far * near) / (far - near);
		//mat4 _projection = { sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, -1, 0, 0, pz, 0 };
		GLfloat yScale = 1.0f / tan(fov / 2.0f);
		GLfloat xScale = yScale / aspect;
		mat4 _projection = {
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, near / (near - far), -1,
			0, 0, near * far / (near - far), 0
		};
		return _projection;
	}
	static mat4 rotationx(GLfloat angle) {
		GLfloat cosangle = cos(angle);
		GLfloat sinangle = sin(angle);
		mat4 _rotationx = {
			1, 0, 0, 0,
			0, cosangle, sinangle, 0,
			0, -sinangle, cosangle, 0,
			0, 0, 0, 1
		};
		return _rotationx;
	}
	static mat4 rotationy(GLfloat angle) {
		GLfloat cosangle = cos(angle);
		GLfloat sinangle = sin(angle);
		mat4 _rotationy = {
			cosangle, 0, -sinangle, 0,
			0, 1, 0, 0,
			sinangle, 0, cosangle, 0,
			0, 0, 0, 1
		};
		return _rotationy;
	}
	static mat4 rotationz(GLfloat angle) {
		GLfloat cosangle = cos(angle);
		GLfloat sinangle = sin(angle);
		mat4 _rotationz = {
			cosangle, sinangle, 0, 0,
			-sinangle, cosangle, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		return _rotationz;
	}
	static mat4 scale(vec3 v) {
		mat4 _scale = {
			v.x, 0, 0, 0,
			0, v.y, 0, 0,
			0, 0, v.z, 0,
			0, 0, 0, 1
		};
		return _scale;
	}
	static mat4 translation(vec3 v) { mat4 _translation = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, v.x, v.y, v.z, 1 }; return _translation; }

	operator GLfloat* () { return fl; }
};
