#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0"; //GLSL vertex shader code

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"; //GLSL fragment shader code

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//x, y are the locations of lower left corner of the window. width and height are measured in pixels
	glViewport(0, 0, width, height);
}

//Process keyboard input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//Initialize OpenGL window
	glfwInit();
	//Indicate maximum OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Indicate minimum OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Explicitly use core profile without backward compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//For OS X
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	//Create OpenGL window, return a GLFW window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Make the window context the main context on current thread
	glfwMakeContextCurrent(window);
	//Load the address of the OpenGL function pointer, which is OS specific
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//Callback for every time the window resizes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window)) //Checks if glfw window is instructed to close
	{
		//input
		processInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Clear the screen setting
		glClear(GL_COLOR_BUFFER_BIT); //Using the setting
		float vertices[] = {
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};//Normalized coordinates
		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};
		unsigned int VBO{};
		unsigned int VAO{};
		unsigned int EBO{};
		glGenBuffers(1, &EBO); //Element Buffer Objects; store indices
		glGenBuffers(1, &VBO); //Vertex buffer objects; send a large number of vertices in the GPU's memory and keep if there's enough memory left.
		glGenVertexArrays(1, &VAO); //Vertex Array Objects
		//1. bind Vertex Array Object
		glBindVertexArray(VAO); 
		//2. Copy vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind buffer to a GL_ARRAY_BUFFER object
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Copy user-defined data into the bound buffer
		//3. copy index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//4. Set vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		unsigned int vertexShader{};
		vertexShader = glCreateShader(GL_VERTEX_SHADER); //Create shader object
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader); //Compile shader
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		unsigned int fragmentShader{};
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		//Create shader program
		unsigned int shaderProgram{};
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//check and call events and swap the buffers
		glfwSwapBuffers(window); //Swap the color buffer
		glfwPollEvents(); //Checks if any events are triggered
	}
	//Clean up resources and properly exit the application
	glfwTerminate();

	return 0;
}