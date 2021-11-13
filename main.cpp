#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "ShadowMapFBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void renderScene(const Shader& shader);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
void renderQuad();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;
unsigned int frameCounter = 0;

// Models
Model* worldModel;
std::vector<Model*> models;

// Positions
glm::vec3 lightPos = glm::vec3(0.0f, 6.0f, 0.0f);
glm::vec3 testCubePos = glm::vec3(6.0f, 4.0f, 6.0f);
glm::vec3 testCube2Pos = glm::vec3(8.0f, 11.0f, 9.0f);

// Font stuffs
struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

std::map<char, Character> Characters;
unsigned int textVAO, textVBO;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bingus window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window " << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Configure global OpenGL states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Compile and set up text shader
	Shader textShader("shaders/text.vert", "shaders/text.frag");
	glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProjection));

	// Initialize the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Failed to initialize FreeType" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else
	{
		FT_Set_Pixel_Sizes(face, 0, 24);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			// generatre texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Store character in map
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Build and compile shader programs
	Shader defaultShader("shaders/lit_diffuse_normal_point_shadow.vert", "shaders/lit_diffuse_normal_point_shadow.frag");
	//Shader lightShader("shaders/light_shader.vert", "shaders/light_shader.frag");
	//Shader shadowMapShader("shaders/shadow_map.vert", "shaders/shadow_map.frag");
	Shader shadowCubeMapShader("shaders/shadowCubeMap.vert", "shaders/shadowCubeMap.frag", "shaders/shadowCubeMap.geom");
	Shader screenShader("shaders/screen.vert", "shaders/screen.frag");

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	// Generate textures
	unsigned int diffuseMap = loadTexture("images/test_texture.png");
	unsigned int normalMap = loadTexture("images/plain_normal.png");

	// Tell opengl for each sampler to which texture unit it belongs to
	defaultShader.use();
	defaultShader.setInt("material.diffuse", 0);
	defaultShader.setInt("material.normal", 1);
	defaultShader.setInt("shadowCubeMap", 2);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	// Load models
	worldModel = Model::loadModel("models/room.obj");
	models.push_back(Model::loadModel("models/cube.obj"));

	// Set up the shadow map framebuffer
	const unsigned int SHADOW_MAP_WIDTH = 256, SHADOW_MAP_HEIGHT = 256;

	// Define screen space projection for debug view
	screenShader.use();
	glm::mat4 screenSpaceProj;
	//screenSpaceProj = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, -1.0f, 1.0f);
	screenSpaceProj = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10.0f);
	screenShader.setMat4("projection", screenSpaceProj);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	screenShader.setMat4("model", model);

	// draw as wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		// -----
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		frameCounter++;

		if (deltaTime >= 1.0f / 30.0f)
		{
			std::string fps = std::to_string((1.0f / deltaTime) * frameCounter);
			std::string title = "opengl window - fps: " + fps;
			glfwSetWindowTitle(window, title.c_str());

			lastFrame = currentFrame;
			frameCounter = 0;
		}
		

		// Process inputs
		// -----
		processInput(window);

		// Rendering
		// -----

		// Render shadow map to framebuffer
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 100.0f;
		lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, near_plane, far_plane);
		glm::mat4 perspectiveLightProjection = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = perspectiveLightProjection * lightView;
		
		// Activate shadow map shader when drawing to shadowMapFBO
		//shadowMapShader.use();
		//shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// TEMP: generating point shadow fbo and shadow depth cube map
		unsigned int pointShadowFBO;
		glGenFramebuffers(1, &pointShadowFBO);

		unsigned int depthCubeMap;
		glGenTextures(1, &depthCubeMap);

		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, pointShadowFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);
		glm::mat4 shadowTransforms[] = {
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		};

		shadowCubeMapShader.use();
		shadowCubeMapShader.setMat4("shadowMatrices[0]", shadowTransforms[0]);
		shadowCubeMapShader.setMat4("shadowMatrices[1]", shadowTransforms[1]);
		shadowCubeMapShader.setMat4("shadowMatrices[2]", shadowTransforms[2]);
		shadowCubeMapShader.setMat4("shadowMatrices[3]", shadowTransforms[3]);
		shadowCubeMapShader.setMat4("shadowMatrices[4]", shadowTransforms[4]);
		shadowCubeMapShader.setMat4("shadowMatrices[5]", shadowTransforms[5]);
		shadowCubeMapShader.setVec3("lightPos", lightPos);
		shadowCubeMapShader.setFloat("far_plane", far_plane);

		//glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		
		glBindFramebuffer(GL_FRAMEBUFFER, pointShadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//shadowMapFBO.bindW();
			renderScene(shadowCubeMapShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch back to default framebuffer
		//glCullFace(GL_BACK);

		// reset viewport
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate regular scene shader
		defaultShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		defaultShader.setMat4("projection", projection);
		defaultShader.setMat4("view", view);

		// Light uniforms
		defaultShader.setVec3("viewPos", camera.Position);
		defaultShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		defaultShader.setFloat("far_plane", far_plane);

		// Material properties
		defaultShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		defaultShader.setFloat("material.shininess", 51.2f);

		// light properties
		defaultShader.setVec3("dirLight.direction", glm::vec3(-0.4f, -1.0f, -0.5f));
		defaultShader.setVec3("dirLight.ambient", glm::vec3(0.4f));
		defaultShader.setVec3("dirLight.diffuse", glm::vec3(1.0f));//off
		defaultShader.setVec3("dirLight.specular", glm::vec3(0.5f));
		 
		// point light 1
		defaultShader.setVec3("pointLights[0].position", lightPos);
		defaultShader.setVec3("pointLights[0].ambient", glm::vec3(0.8f));
		defaultShader.setVec3("pointLights[0].diffuse", glm::vec3(1.0f));
		defaultShader.setVec3("pointLights[0].specular", glm::vec3(1.0f));
		defaultShader.setFloat("pointLights[0].constant", 1.0f); // Set falloff values to distance of 100 https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		defaultShader.setFloat("pointLights[0].linear", 0.045);
		defaultShader.setFloat("pointLights[0].quadratic", 0.0075);

		// Drawing code
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		//shadowMapFBO.bindR(GL_TEXTURE2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		renderScene(defaultShader);

		//// draw the a quad
		glDisable(GL_DEPTH_TEST);
		screenShader.use();

		//screenSpaceProj = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, -1.0f, 1.0f);
		screenSpaceProj = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10.0f);
		screenShader.setMat4("projection", screenSpaceProj);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		screenShader.setMat4("model", model);

		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		renderQuad();
		//
		// Draw text!!!!!!!
		//RenderText(textShader, "FPS: " + std::to_string(fps), 0.0f, SCREEN_HEIGHT - 24.0f, 1.0f, glm::vec3(1.0f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate resources
	glDeleteVertexArrays(1, &textVAO);
	glDeleteBuffers(1, &textVBO);
	//glDeleteVertexArrays(1, &quadVAO);
	//glDeleteBuffers(1, &quadVBO);

	glfwTerminate();
	return 0;
}

void renderScene(const Shader& shader)
{
	// Draw map model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	worldModel->draw();

	// Draw cube
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f));
	model = glm::translate(model, testCubePos);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(.3, .2, .5));
	shader.setMat4("model", model);
	models[0]->draw();

	// Draw second cube
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.5f));
	model = glm::translate(model, testCube2Pos);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(.1, .4, .3));
	shader.setMat4("model", model);
	models[0]->draw();

	// Draw light cube
	//model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(1.0f));
	//model = glm::translate(model, lightPos);
	//shader.setMat4("model", model);
	//models[0]->draw();
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}


void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int quadVAO = 0, quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		// Set up vertex data and buffers
		float quadVertices[] = {
			-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,		1.0f, 0.0f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
