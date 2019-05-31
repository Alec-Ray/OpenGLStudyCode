#include "includes\glad\glad.h"
#include "includes\glfw\glfw3.h"
#include <iostream>
#include "includes\Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "includes\stb_image.h"

#include "includes\glm\glm.hpp"
#include "includes\glm\gtc\matrix_transform.hpp"
#include "includes\glm\gtc\type_ptr.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef  _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //  _APPLE_

	GLFWwindow* window = glfwCreateWindow(1000, 800, "WjgOGL", NULL, NULL);
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

	Shader MyShader("includes/shader.vs", "includes/shader.fs");

	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f    // 左上
	};

	//索引
	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	//顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//顶点数组对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//顶点索引对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//绑定EBO
	//VAO绑定时正在绑定的EBO会被保存为VAO的元素缓冲对象,绑定VAO的同时也会自动绑定EBO。
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//设置顶点(位置)属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//设置颜色属性指针
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//设置纹理坐标属性指针
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//翻转图片的y轴
	stbi_set_flip_vertically_on_load(true);

	//生成第一个纹理
	unsigned int texture01;
	glGenTextures(1, &texture01);
	glBindTexture(GL_TEXTURE_2D, texture01);

	//为当前的绑定的纹理对象设置环绕，过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}

	//释放第一个纹理的空间
	stbi_image_free(data);

	//生成第二个纹理
	unsigned int texture02;
	glGenTextures(1, &texture02);
	glBindTexture(GL_TEXTURE_2D, texture02);

	//为当前的绑定的纹理对象设置环绕，过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	data = stbi_load("textures/face.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture" << std::endl;
	}

	//释放第二个纹理的空间
	stbi_image_free(data);


	//解除绑定，注意解绑顺序
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//必须在VAO之后解绑EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
	glBindTexture(GL_TEXTURE_2D, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	MyShader.use();
	glUniform1i(glGetUniformLocation(MyShader.ID, "texture01"), 0);
	MyShader.setInt("texture02", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.6f, 0.5f ,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture01);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture02);

		glm::mat4 transform = glm::mat4(1.0f);
		////绕z轴旋转90度
		//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		////x,y,z都缩小一半
		//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

		//x轴向右移动0.5，y轴向下移动0.5
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f,0));
		//随着时间绕z轴旋转
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		MyShader.use();

		//将矩阵传给顶点着色器
		unsigned int transformLoc = glGetUniformLocation(MyShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture01);
	glDeleteTextures(1, &texture02);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//按空格键进入线框模式(Wireframe Mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}		

}
