/* NOTE:
	Stuff you will need:
		glew 1.10.0 in ./glew-1.10.0
		glfw 3.0.2 WIN64 in ./glfw-3.0.2.bin.WIN64
		glew32.dll in the executable directory

	Directories to include in project settings:
		include:
			(GLEW)/include
			(GLFW)/include
		lib:
			(GLEW)/lib/Release/x64
				or whichever
			(GLFW)/lib-msvc110
				or whichever

	Build Options:
		MSVC "Code Generation" needs Multi-Threaded DLL (for GLFW I think)
			and this means you ought to include the MSVC redist DLLs with the EXE
		'could probably fix this but it's no biggy yet

	Coordinate System:
		right-handed, up: y, right: x, in: z

	Todo:
		error checking on GL commands
*/

#include "headers.h"

#include "ccube.h"

void mainFunc(int argc, unsigned short **argvw) {
	if(!glfwInit()) {
		// todo
	} else {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		GLFWwindow *window = glfwCreateWindow(1024, 768, "Title", NULL, NULL);
		if(!window) {
			// todo
		} else {
			glfwMakeContextCurrent(window);
			if(GLEW_OK != glewInit()) {
				// todo
			} else {
				cCube cube;
				mat4 projection_matrix = mat4::projectionrh();
				mat4 view_matrix = mat4::lookatrh(vec3::make(100, -100, 300), vec3::make(0, 0, 0), vec3::make(0, 1, 0));
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				while(!glfwWindowShouldClose(window)) {
					glClearColor(0, 0, 0, 1);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					cube.setProjectionMatrix(projection_matrix);
					cube.setViewMatrix(view_matrix);
					cube.preRender();
					for(unsigned int y = 0; y < 100; ++y) {
						for(unsigned int x = 0; x < 100; ++x) {
							GLfloat xf = (GLfloat)x * 3.0f - 150.0f;
							GLfloat yf = (GLfloat)y * 3.0f - 150.0f;
							cube.render(vec3::make(xf, yf, 0.0f));
						}
					}
					glfwSwapBuffers(window);
					glfwPollEvents();
				}
			}
		}
		glfwTerminate();
	}
}