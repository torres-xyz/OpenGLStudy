#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//Vertex shader source code
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)glsl";

//Fragment shader source code
const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
}
)glsl";


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Vertice coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, //upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, //inner down
	};

	//Defining the triangles
	GLuint indices[] =
	{
		0,3,5,
		3,2,4,
		5,4,1
	};

	GLFWwindow* window = glfwCreateWindow(800, 800, "Open GL Journey", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 800);


	//Create and compile the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//now that the shaders are already in the program we can delete them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VAO, VBO, EBO; //Vertex Array Object, Vertex Buffer Object

	glGenVertexArrays(1, &VAO); //must be generated before VBO.
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Make the VAO the current Vertex Array Object by binding it.
	glBindVertexArray(VAO);

	//Bind the VBO specifying	it's a GL_Array_Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//Configure the Vertex Attribute so that OpenGL knows how to read the VBO.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable the Vertex attribute so that OpenGL know how to use it.
	glEnableVertexAttribArray(0);

	//Bind both the VBO and VAO to 0 to that we don't accidentally modify them
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Specify the background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	//Swap the back and front buffers
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		//Specify the background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clean the back buffer and the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		//Tell OpenGL which shader program we want to use
		glUseProgram(shaderProgram);
		//Bind the VAO so OpenGL knows to use it.
		glBindVertexArray(VAO);

		//Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//Swap the back and front buffers
		glfwSwapBuffers(window);


		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}