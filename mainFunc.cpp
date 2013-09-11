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
*/
#include "GL/glew.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
#include "GLFW/glfw3.h"
#pragma comment(lib, "glfw3.lib")

void mainFunc(int argc, unsigned short **argvw) {
	if(!glfwInit()) {
		// todo
	} else {
		GLFWwindow *window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
		if(!window) {
			// todo
		} else {
			glfwMakeContextCurrent(window);
			if(GLEW_OK != glewInit()) {
				// todo
			} else {
				while(!glfwWindowShouldClose(window)) {
					// todo: render here
					glfwSwapBuffers(window);
					glfwPollEvents();
				}
			}
		}
		glfwTerminate();
	}
}