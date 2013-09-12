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
#include "clife.h"

void mainFunc(int argc, unsigned short **argvw) {
	if(!glfwInit()) {
		// todo
	} else {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 16);
		GLFWwindow *window = glfwCreateWindow(1024, 768, "Conway's Life", NULL, NULL);
		if(!window) {
			// todo
		} else {
			glfwMakeContextCurrent(window);
			if(GLEW_OK != glewInit()) {
				// todo
			} else {
				// prep for the render loop
				unsigned int width = 400, height = 400;

				cLife life(width, height);
				life.randomize();

				cCube cube;
				
				mat4 projection_matrix = mat4::projectionrh();
				mat4 view_matrix = mat4::lookatrh(vec3::make(0.25f * width, -0.25f * height, 1.5f * width), vec3::make(0.125f * width, -0.0625f * height, 0), vec3::make(0, 1, 0));
				
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				
				// render loop
				while(!glfwWindowShouldClose(window)) {
					glClearColor(0, 0, 0, 1);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
					cube.setProjectionMatrix(projection_matrix);
					cube.setViewMatrix(view_matrix);
					cube.preRender();

					// render blocks that are alive and recently dead
					for(unsigned int y = 0; y < height; ++y) {
						for(unsigned int x = 0; x < width; ++x) {
							GLfloat xf = (GLfloat)x * 2.0f - GLfloat(width);
							GLfloat yf = (GLfloat)y * 2.0f - GLfloat(height);
							unsigned char val = life.getold(x, y);
							if(val > 48) {
								GLfloat valf = (GLfloat)val / 255.0f;
								cube.render(vec3::make(xf, yf, 0.0f), vec3::make(0.6f, 0.9f, 0.6f) * valf);
							}
						}
					}

					life.iterate();
					if(life.isstalled()) life.randomize();

					glfwSwapBuffers(window);
					glfwPollEvents();
				}
			}
		}
		glfwTerminate();
	}
}