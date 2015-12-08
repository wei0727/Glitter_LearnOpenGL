// Local Headers
#include "glitter.hpp"
#include "Shader.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

const string SRCPATH = "D:\\Wei\\CSIE\\OpenGL\\Glitter_LearnOpenGL\\Glitter\\Sources\\";
const string TEXPATH = "D:\\Wei\\CSIE\\OpenGL\\Glitter_LearnOpenGL\\Glitter\\Textures\\";

// Goble variable... should be removed by class implementation
bool keys[1024];

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Exit if press Escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Use for Camera Control
	if (key >= 0 && key < 1024){
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Move Camera Position
void camera_movement(glm::vec3 &cameraPos, glm::vec3 &cameraFront, glm::vec3 &cameraUp){
	const float speed = 0.01f;
	if (keys[GLFW_KEY_W])
		cameraPos += speed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= speed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

int main(int argc, char * argv[]) {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	glViewport(0, 0, mWidth, mHeight);

	// Set Callback Functions
	glfwSetKeyCallback(mWindow, key_callback);

	// Load Shaders
	string vs_path = SRCPATH + "vertex_shader.glsl";
	string fs_path = SRCPATH + "fragment_shader.glsl";
	Shader mShader(vs_path, fs_path);

	// Load textures
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, n;
	//unsigned char* image = stbi_load((TEXPATH + "container.jpg").c_str(), &width, &height, &n, 0);
	unsigned char* image = stbi_load((TEXPATH + "awesomeface.jpg").c_str(), &width, &height, &n, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Vertices
	GLfloat vertices[] = {
		// Positions		// Colors			// Texture Coords
		0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// Top Right
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		// Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// Bottom Left
		-0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		// Top Left 
		0.5f, 0.5f, -1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// Top Right Back
		0.5f, -0.5f, -1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		// Bottom Right Back
		-0.5f, -0.5f, -1.0f,0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// Bottom Left Back
		-0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,	0.0f, 1.0f		// Top Left  Back
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,	// First Triangle
		1, 2, 3,	// Second Triangle
		0, 7, 3,
		0, 4, 7,
		2, 1, 6,
		1, 6, 5,
		0, 1, 5,
		0, 5, 4,
		2, 6, 7,
		2, 7, 3,
		4, 7, 5,
		6, 7, 5
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Poisitions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Enable Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Camera setting
	glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);


    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
		glfwPollEvents();
		camera_movement(cameraPos, cameraFront, cameraUp);
        // Background Fill Color
        glClearColor(0.0f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing
		// Shader Program
		mShader.Use();
		// Transformation Matrix
		//glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime()*50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection = glm::perspective(45.0f, (float)mWidth / (float)mHeight, 0.1f, 100.0f);
		GLuint modelLoc = glGetUniformLocation(mShader.Program, "model");
		GLuint viewLoc = glGetUniformLocation(mShader.Program, "view");
		GLuint projectionLoc = glGetUniformLocation(mShader.Program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(mShader.Program, "ourTexture"), 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
    }   
	
	// Free Memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
    return EXIT_SUCCESS;
}
