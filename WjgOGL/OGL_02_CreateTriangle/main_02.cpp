#include <glad\glad.h>
#include <glfw\glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char *vertexShaderSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

const char *fragmentShaderSource =  
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef  _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //  _APPLE_

	GLFWwindow* window = glfwCreateWindow(1000, 700, "WjgOGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//改变调用线程上的指定窗口的上下文为当前窗口
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initilize glad" << std::endl;
		return -1;
	}

	//编译顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int shaderSuccess;
	char shaderErrorLog[512];

	//检测顶点着色器是否编译成功,GL_COMPILE_STATUS：编译状态
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		//存储编译错误的消息
		glGetShaderInfoLog(vertexShader, 512, NULL, shaderErrorLog);
		std::cout << "vertex shader compile error and log:" << shaderErrorLog << std::endl;
	}

	//编译片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//检测片段着色器是否编译成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, shaderErrorLog);
		std::cout << "fragment shader compile error and log:" << shaderErrorLog << std::endl;
	}

	//着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//如果要使用我们自定义的两个着色器，必须要把它们链接到一个着色器程序
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//检查链接着色器的程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, shaderErrorLog);
		std::cout << "shader program link error and log:" << shaderErrorLog << std::endl;
	}

	//卸磨杀驴，两个着色器对象已经链接到一个着色器程序上了，我们不需要他们了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//三个顶点,NDC坐标系下的位置
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	//顶点缓冲对象,顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//顶点数组对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	/*VAO本身并没有存储顶点的相关属性数据，这些信息是存储在VBO中的，
	VAO相当于是对很多个VBO的引用，把一些VBO组合在一起作为一个对象统一管理。
	执行VAO绑定之后其后的所有VBO配置都是这个VAO对象的一部分，
	可以说VBO是对顶点属性信息的绑定，VAO是对很多个VBO的绑定。
	*/
	//绑定VAO
	glBindVertexArray(VAO);

	//绑定VBO,我们之后使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把定义好的顶点数据复制到缓冲的内存中
	/*三角形的位置数据不会改变，每次渲染调用时都保持原样，所以它的使用类型最好是GL_STATIC_DRAW。
	如果，比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，
	这样就能确保显卡把数据放在能够高速写入的内存部分。*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*	
	例如：layout( location=index ) in vec4 position;
	如果这个index和glVertexAttribPointer的第一个参数一样，
	那么相关缓存区的数据就会传递到这个position变量中去。
	glVertexAttribPointer执行后，会影响改变VAO的状态，VBO会被复制保存到VAO中。
	之后如果改变了当前所绑定的缓存对象，也不会改变到VAO里的对象
	*/
	//设置顶点属性指针,第一个0就是顶点着色器里面写的顶点属性位置值
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//以顶点属性位置值0[顶点着色器里面也写了:layout(location=0)]作为参数，启用顶点属性
	glEnableVertexAttribArray(0);

	//解除VBO和VAO的绑定，注意解绑顺序
	//之所以可以解除VBO绑定是因为执行glVertexAttribPointer后，OpenGL已经找到了VBO的顶点属性位置值
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//解除VAO绑定防止这个VAO被其他的VAO的调用修改,但事实上这很少发生
	glBindVertexArray(0);


	//------------------------------------------------------------------------------------------------
	/*
	前面做的一切都是等待这一刻，一个储存了我们顶点属性配置和应使用的VBO的顶点数组对象。
	一般当你打算绘制多个物体时，你首先要生成/配置所有的VAO（和必须的VBO及属性指针)，
	然后储存它们供后面使用。当我们打算绘制物体的时候就拿出相应的VAO，
	绑定它，绘制完物体后，再解绑VAO。
	*/
	//------------------------------------------------------------------------------------------------


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.6f, 0.5f ,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//绘制三角形
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//我们目前只有一个VAO,所以不需要每一次都解绑
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
