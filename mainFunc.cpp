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
				unsigned int width = 400, height = 300;

				cLife life(width, height);
				life.randomize();

				cCube cube;
				
				mat4 projection_matrix = mat4::projectionrh();
				mat4 view_matrix = mat4::lookatrh(vec3::make(0.25f * width, -0.25f * height, 1.5f * width), vec3::make(0.125f * width, -0.0625f * height, 0), vec3::make(0, 1, 0));
				
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glfwSwapInterval(1);
				
				double time = glfwGetTime();
				double frametime = 1.0 / 100.0; // just to get things started

				std::list<std::vector<vec3>*> history;
				unsigned int maxhistory = 64; // or ~whatever~

				// render loop
				while(!glfwWindowShouldClose(window)) {
					glClearColor(0, 0, 0, 1);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
					cube.setProjectionMatrix(projection_matrix);

					GLfloat leftright = sin(time / 2.0) * 0.4f;
					GLfloat updown = sin(time / 3.1456) * 0.1f;
					GLfloat scale = 1.0f / width;
					view_matrix = mat4::scale(vec3::make(scale));
					view_matrix = mat4::multiply(view_matrix, mat4::translation(vec3::make(0, 0, -1.5f)));
					view_matrix = mat4::multiply(mat4::rotationy(leftright), view_matrix);
					view_matrix = mat4::multiply(mat4::rotationx(updown), view_matrix);

					cube.setViewMatrix(view_matrix);
					
					std::vector<vec3> *player = nullptr;
					if(!history.empty()) {
						unsigned int whichlayer = 0;
						for(std::list<std::vector<vec3>*>::iterator it = history.begin(); it != history.end(); ++it) {
							cube.preRender(vec3::make(0.1f + 0.6f * (GLfloat)(maxhistory - whichlayer) / (GLfloat)maxhistory));
							std::vector<vec3> &layer = **it;
							GLfloat zf = -2.0f * (GLfloat)++whichlayer;
							for(unsigned int i = 0; i < layer.size(); ++i) {
								vec3 &location = layer[i];
								location.z = zf;
								cube.render(layer[i]);
							}
						}
						// if size is less than max then add an empty to the front
						if(history.size() < maxhistory) {
							player = new std::vector<vec3>;
							history.push_front(player);
						} else {
							player = history.back();
							history.pop_back();
							player->resize(0);
							history.push_front(player);
						}
					} else {
						player = new std::vector<vec3>;
						history.push_front(player);
					}

					std::vector<vec3> &layer = *player;
					cube.preRender(vec3::make(1));
					// render blocks that are alive and recently dead
					for(unsigned int y = 0; y < height; ++y) {
						for(unsigned int x = 0; x < width; ++x) {
							GLfloat xf = (GLfloat)x * 2.0f - GLfloat(width);
							GLfloat yf = (GLfloat)y * 2.0f - GLfloat(height);
							if(life.isalive(x, y)) {
								vec3 location = { xf, yf, 0 };
								cube.render(location);
								layer.push_back(location);
							}
						}
					}

					if(glfwGetKey(window, GLFW_KEY_SPACE)) {
						life.randomize();
					} else if(glfwGetKey(window, GLFW_KEY_0)) {
						life.special(0);
					} else if(glfwGetKey(window, GLFW_KEY_1)) {
						life.special(1);
					} else if(glfwGetKey(window, GLFW_KEY_2)) {
						life.special(2);
					} else if(glfwGetKey(window, GLFW_KEY_3)) {
						life.special(3);
					} else {
						life.iterate();
					}
					//if(life.isstalled()) life.randomize();

					glfwSwapBuffers(window);
					glfwPollEvents();

					double now = glfwGetTime();
					frametime = now - time;
					time = now;
				}
			}
		}
		glfwTerminate();
	}
}