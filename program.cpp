#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "ResourceManager.h"
#include "Game.h"
#include "Debug.h"

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Game testGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment this line in a release build!
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "opengl-engine test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window " << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// enable OpenGL debug context if context allows for debug context
	if (GLAD_GL_KHR_debug) std::cout << "KHR_debug supported" << std::endl;

	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "OpenGL debug context present" << std::endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	// Configure global OpenGL states
	// -------------------------------
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize the game
	testGame.init();

	// Delta time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	unsigned int frameCounter = 0;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe rendering
	while (!glfwWindowShouldClose(window))
	{
		// calculate delta time
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		std::string windowTitleDt = "opengl-engine test - dt: " + std::to_string(deltaTime);
		glfwSetWindowTitle(window, windowTitleDt.c_str());
		glfwPollEvents();

		// manage user input
		// -----------------
		testGame.processInput(deltaTime);

		// update game state
		// -----------------
		testGame.update(deltaTime);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		testGame.render();

		glfwSwapBuffers(window);
	}

	// deallocate resources
	ResourceManager::clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) testGame.Keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			testGame.Keys[key] = false;
			testGame.KeysProcessed[key] = false;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}