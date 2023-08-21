#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//Create GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	
	//window tasks loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float vertices[] = {
			-0.8f, 0.5f, 0.0f,
			-0.8f, -0.5f, 0.0f,
			-0.3f, -0.5f, 0.0f, //first triangle
			
			-0.2f, 0.5f, 0.0f,
			-0.2f, -0.5f, 0.0f,
			0.3f, -0.5f, 0.0f //second triangle
		};
		unsigned int VBO{};
		unsigned int VAO{};
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		//1. Bind vertex array object
		glBindVertexArray(VAO);
		//2. Copy vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//3. Set vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//Create vertex shader
		unsigned int vertexShader{};
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0"; //GLSL vertex shader code
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		int success{};
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
		}
		//Create fragment shader
		unsigned int fragmentShader{};
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0"; //GLSL fragment shader code
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
		}
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
			std::cout << "Shader program link failed " << infoLog << std::endl;
		}
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//post rendering tasks
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Terminate the program
	glfwTerminate();

	return 0;
}