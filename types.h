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
	static vec3 normal(const vec3 &in) {
		GLfloat invl = 1.0f / sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
		vec3 v = { in.x * invl, in.y * invl, in.z * invl };
		return v;
	}
	vec3 normal() { return normal(*this); }
	static vec3 cross(const vec3 &left, const vec3 &right) { vec3 v = { left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y }; return v; }
	vec3 cross(const vec3 &rvalue) const { return cross(*this, rvalue); }
	static GLfloat dot(const vec3 &left, const vec3 &right) { return left.x * right.x + left.y * right.y + left.z * right.z; }
	GLfloat dot(const vec3 &rvalue) const { return dot(*this, rvalue); }
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
		GLfloat m11, m21, m31, m41;
		GLfloat m12, m22, m32, m42;
		GLfloat m13, m23, m33, m43;
		GLfloat m14, m24, m34, m44;
	};
	struct { vec4 c1, c2, c3, c4; };
	GLfloat f[16];
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
	static mat4 projectionrh(GLfloat near = 1.0f, GLfloat far = 1000.0f, GLfloat fov = 1.221730f, GLfloat aspect = (4.0f / 3.0f)) {
		GLfloat range = tan(fov * 0.5f) * near;
		GLfloat sx = (2.0f * near) / (range * aspect + range * aspect);
		GLfloat sy = near / range;
		GLfloat sz = -(far + near) / (far - near);
		GLfloat pz = -(2.0f * far * near) / (far - near);
		mat4 _projection = { sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, -1, 0, 0, pz, 0 };
		return _projection;
	}
	static mat4 translation(vec3 v) { mat4 _translation = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, v.x, v.y, v.z, 1 }; return _translation; }
	operator GLfloat* () { return f; }
};
