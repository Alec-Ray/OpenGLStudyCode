#include "includes\glad\glad.h"
#include "includes\glfw\glfw3.h"
#include <iostream>

#include "includes\Shader.h"
#include "includes\camera.h"
#include "includes\model.h"

#include "includes\glm\glm.hpp"
#include "includes\glm\gtc\matrix_transform.hpp"
#include "includes\glm\gtc\type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "includes\stb_image.h"

#pragma comment ( lib,"glfw3.lib" ) 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

const unsigned int Screen_Width  = 1000;
const unsigned int Screen_Height = 800;

float lastX = Screen_Width/2.0f;
float lastY = Screen_Height/2.0f;

float deltaTime = 0.0f;		
float lastFrame = 0.0f;	

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool firstMouse = true;

glm::vec3 lightPos(1.0f, 1.0f, 2.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef  _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //  _APPLE_

	GLFWwindow* window = glfwCreateWindow(Screen_Width, Screen_Height, "WjgOGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initilize glad" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
	//sfail��ģ�����ʧ��ʱ��ȡ����Ϊ��
	//dpfail��ģ�����ͨ��������Ȳ���ʧ��ʱ��ȡ����Ϊ��
	//dppass��ģ����Ժ���Ȳ��Զ�ͨ��ʱ��ȡ����Ϊ��

	//����ģ�����
	glEnable(GL_STENCIL_TEST);
	//����GL_NOTEQUAL��ͨ��ģ����ԣ��ο�ֵΪ1(ref)
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//GL_KEEP	���ֵ�ǰ�����ģ��ֵ
	//GL_REPLACE	��ģ��ֵ����ΪglStencilFunc�������õ�refֵ
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	
	Shader MyShader("includes/shader.vs", "includes/shader.fs");
	Shader OutlineShader("includes/shader.vs", "includes/OutlineShader.fs");
	
	float cubeVertices[] = {
		// ��������          // ��������
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//����
	float planeVertices[] = {	
		//���������������1��ͼ���ڱ������ظ����֣�GL_REPEAT
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
	};

	//����VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// ����VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	
	unsigned int cubeTexture = loadTexture("textures/container2.png");
	unsigned int floorTexture = loadTexture("textures/metal.png");

	MyShader.use();
	MyShader.setInt("texture1", 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*
	ԭ�������
		��������ʵ���ǻ�������ͼ�Ρ�һ������£�����ͼ�ζ��ử��������Ȼ��Ȳ��Իᵲסһ���֣���
		���ڼ�����ģ����ԣ���ô��һ������ͼ�ε�ģ��ֵȫ�滻Ϊ1(�ؼ�����GL_REPLACE �� �����������룩��
		�ڶ���ͼ�Σ��Ŵ�һ��㣬������ɫ����ɫ������ôһ������£�������һ����һȦ�ĵ�ɫģ�͵�ס�˵�һ��ͼ�Σ���Ȳ��ԵĹ�����
		���¹�����Ȳ��ԣ�ͼ����Ȼ��һƬ�̵ģ�����Ƶĸ���ǰ���Ƶģ���
		�������õ�һ��ͼ���ǲ���ģ��ֵȫ��1���ص㣬���ú�������Ϊ��ģ��ֵ������1ʱ���ڶ���ͼ�ν��л��ơ���ôЧ�����ǣ�
		��һ��ͼ���������ƣ��ڶ���ͼ�ν��бȵ�һ��ͼ�δ���ǲ��ֻ��Ƴ��������Կ���ȥ���Ƕ���һ����Ե��
	*/

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window);

		glClearColor(0.1, 0.1f, 0.1f ,1.0f);
		//���ģ�建��
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		MyShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Screen_Width / (float)Screen_Height, 0.1f, 100.0f);
		MyShader.setMat4("view", view);
		MyShader.setMat4("projection", projection);

		//�����������ĵĶ�����ɫ��,���Լ���ʹ��֮ǰ�Ĺ۲��߾����ͶӰ����ģ�;��������ϵ����������
		OutlineShader.use();
		OutlineShader.setMat4("view", view);
		OutlineShader.setMat4("projection", projection);

		MyShader.use();

		//�Ȼ��Ƶ��棬���ҹر�ģ�建���д��
		glStencilMask(0x00);
		//����
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		MyShader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//��ʼ�����������������ӣ�����ģ�建���д�룬GL_ALWAYS��˼Ϊ����ͨ��ģ�����
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		//����
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture); 	
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		MyShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		MyShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//���������Ŵ�����ӣ��ر�ģ�建���д��
		//GL_NOTEQUAL��˼Ϊ�����ڲ�ͨ��ģ�����,�����ڻ����������������ӵ�ʱ���Ѿ������ǵ�Ƭ�ε�ģ�建����Ϊ��1
		//�ر���Ȳ��ԣ���Ϊ�˲�Ӱ��������ʾ
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		OutlineShader.use();
		//�����Ŵ�����ӣ�ֻ��������ͨ����ģ�����
		float scale = 1.05;
		glBindVertexArray(cubeVAO);
		//����Ҫ�õ�����
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//�Ŵ�
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		OutlineShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//�Ŵ�
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		OutlineShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);		
		glBindVertexArray(0);

		//�ٴο�����ģ�建��д��
		glStencilMask(0xFF);
		//�ٴο�����Ȳ���
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << xpos << "+" << ypos << std::endl;
	
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(const char * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
