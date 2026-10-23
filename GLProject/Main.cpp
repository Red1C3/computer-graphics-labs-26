// GLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

int main(int argc, char *argv[])
{
	if (argc > 0)
	{
		std::filesystem::path exePath = std::filesystem::canonical(argv[0]).parent_path();
		std::filesystem::current_path(exePath);
	}
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(800, 800, "Lecture 2", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Shader ourShader("./shaders/vs/L2.vs", "./shaders/fs/color.fs");

	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.366f, 0.0f, 0.0f, 0.0f, 1.0f};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 transformation = glm::mat4(1.0f);
		// transformation = glm::translate(transformation, glm::vec3(0.0f, 0.1f * glm::cos((float)glfwGetTime()*2), 0.0f));
		// transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		// transformation = glm::scale(transformation, glm::vec3(0.5, 0.5, 0.5));
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));

		// render the triangle
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}