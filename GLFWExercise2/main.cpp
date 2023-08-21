#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, 800, 600);
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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise2", NULL, NULL);
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

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float vertices1[] = {
			-0.8f, 0.5f, 0.0f,
			-0.8f, -0.5f, 0.0f,
			-0.3f, -0.5f, 0.0f
		};
		float vertices2[] = {
			-0.2f, 0.5f, 0.0f,
			-0.2f, -0.5f, 0.0f,
			0.3f, -0.5f, 0.0f
		};
		
		unsigned int VAO{};
		unsigned int VBO{};
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

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

		unsigned int shaderProgram{};
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "Shader program link failed: " << infoLog << std::endl;
		}
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		unsigned int VAO1{};
		unsigned int VBO1{};
		glGenBuffers(1, &VBO1);
		glGenVertexArrays(1, &VAO1);
		glBindVertexArray(VAO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		unsigned int fragmentShader1{};
		fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentShaderSource1 = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			" FragColor = vec4(0.9f, 0.9f, 0.05f, 1.0f);\n"
			"}\0"; //GLSL fragment shader code
		glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
		glCompileShader(fragmentShader1);
		glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
			std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
		}

		unsigned int shaderProgram1{};
		shaderProgram1 = glCreateProgram();
		glAttachShader(shaderProgram1, vertexShader);
		glAttachShader(shaderProgram1, fragmentShader1);
		glLinkProgram(shaderProgram1);
		glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
			std::cout << "Shader program link failed: " << infoLog << std::endl;
		}
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}