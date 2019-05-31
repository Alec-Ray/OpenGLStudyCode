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
	//�ı�����߳��ϵ�ָ�����ڵ�������Ϊ��ǰ����
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initilize glad" << std::endl;
		return -1;
	}

	Shader MyShader("includes/shader.vs", "includes/shader.fs");

	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,   // ����
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,   // ����
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f    // ����
	};

	//����
	unsigned int indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	//���㻺�����
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//�����������
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//������������
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//��EBO
	//VAO��ʱ���ڰ󶨵�EBO�ᱻ����ΪVAO��Ԫ�ػ������,��VAO��ͬʱҲ���Զ���EBO��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//���ö���(λ��)����ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������ɫ����ָ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//����������������ָ��
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//��תͼƬ��y��
	stbi_set_flip_vertically_on_load(true);

	//���ɵ�һ������
	unsigned int texture01;
	glGenTextures(1, &texture01);
	glBindTexture(GL_TEXTURE_2D, texture01);

	//Ϊ��ǰ�İ󶨵�����������û��ƣ����˷�ʽ
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

	//�ͷŵ�һ������Ŀռ�
	stbi_image_free(data);

	//���ɵڶ�������
	unsigned int texture02;
	glGenTextures(1, &texture02);
	glBindTexture(GL_TEXTURE_2D, texture02);

	//Ϊ��ǰ�İ󶨵�����������û��ƣ����˷�ʽ
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

	//�ͷŵڶ�������Ŀռ�
	stbi_image_free(data);


	//����󶨣�ע����˳��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//������VAO֮����EBO
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
		////��z����ת90��
		//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		////x,y,z����Сһ��
		//transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

		//x�������ƶ�0.5��y�������ƶ�0.5
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f,0));
		//����ʱ����z����ת
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		MyShader.use();

		//�����󴫸�������ɫ��
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
		//���ո�������߿�ģʽ(Wireframe Mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}		

}
