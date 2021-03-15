#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLSLShader.h"

#include "mathlib.h"

int g_width = 800;
int g_height = 600;
float g_angle = 0.0f;
float g_angle_delta = 0.02f;

static void err_callback(int err_code, const char* desc);
static void framebuffer_callback(GLFWwindow* window, int width, int height);

static const std::map<int, int> wndHints = {
	{ GLFW_RED_BITS,		8  },
	{ GLFW_GREEN_BITS,		8  },
	{ GLFW_BLUE_BITS,		8  },
	{ GLFW_ALPHA_BITS,		8  },
	{ GLFW_DEPTH_BITS,		24 },
	{ GLFW_STENCIL_BITS,	8  },
	{ GLFW_CONTEXT_VERSION_MAJOR, 3 },
	{ GLFW_CONTEXT_VERSION_MINOR, 3 },
	{ GLFW_OPENGL_FORWARD_COMPAT,	GLFW_TRUE },
	{ GLFW_OPENGL_PROFILE,			GLFW_OPENGL_CORE_PROFILE },
	{ GLFW_OPENGL_DEBUG_CONTEXT,	GL_TRUE }
};


#define RandComponent (rand() % 255) / 255.0f
#define RandColor RandComponent, RandComponent, RandComponent

int main(int argc, char* argv[]) {
	glfwSetErrorCallback(err_callback);
	if (!glfwInit()) {
		fprintf(stderr, "Couldn't initialize GLFW!\n");
		exit(EXIT_FAILURE);
	}

	for (const auto& [hint, value] : wndHints) {
		glfwWindowHint(hint, value);
	}

	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "Rendering window", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Couldn't create glfw window!\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSwapInterval(1);

	GLSLShader sh("test.vs", "test.fs");
	srand(123);
	float vertices[] = {
		-0.5, -0.5,  0.5, RandColor,
		-0.5,  0.5,  0.5, RandColor,
		-0.5, -0.5, -0.5, RandColor,
		-0.5,  0.5, -0.5, RandColor,
		 0.5, -0.5,  0.5, RandColor,
		 0.5,  0.5,  0.5, RandColor,
		 0.5, -0.5, -0.5, RandColor,
		 0.5,  0.5, -0.5, RandColor
	};
	
	unsigned int indices[] = {
		0, 1, 2,
		3, 6, 2,
		7, 4, 6,
		5, 0, 4,
		6, 0, 2,
		3, 5, 7,
		1, 3, 2,
		3, 7, 6,
		7, 5, 4,
		5, 1, 0,
		6, 4, 0,
		3, 1, 5
	};

	GLuint VBO, EBO, VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.5f, 0.9f, 1.0f);

	vec3f CamPos = vec3f(5.f, 10.f, 5.f);
	vec3f CamTarget = vec3f(0.f, 0.f, 0.f);
	vec3f CamUp = vec3f(0.f, 1.f, 0.f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			g_angle += g_angle_delta;
		}
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			g_angle = .0f;
		}

		sh.Use();

		mat4f lookat;
		MatMakeLookAt(lookat, CamTarget, CamPos, CamUp);

		sh.SetConstant(GLSLCONSTANTS::MAT4F, "lookat", lookat.value);
	
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

static void err_callback(int err_code, const char* desc) {
	fprintf(stderr, "Error[%d]: %s\n", err_code, desc);
}

void framebuffer_callback(GLFWwindow* window, int width, int height)
{
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}
