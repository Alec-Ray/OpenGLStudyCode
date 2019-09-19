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
	
	Shader MyShader("includes/shader.vs", "includes/shader.fs");
	//֡���������ͼ��Shader
	Shader ScreenShader("includes/screenShader.vs","includes/screenShader.fs");
	
	float cubeVertices[] = {
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

	float planeVertices[] = {	
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
	};

	//֡�������������������ε����꣬��Ļ�ռ��µı�׼���豸���꣬����Ҫ�پ������־���任
	float quadVertices[] = { 
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
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
	//֡���������VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	unsigned int cubeTexture = loadTexture("textures/container2.png");
	unsigned int floorTexture = loadTexture("textures/metal.png");

	MyShader.use();
	MyShader.setInt("texture1", 0);

	//����pixel shader texture
	ScreenShader.use();
	ScreenShader.setInt("screenTexture", 0);

	//֡��������
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//����һ������ͼ�����ǽ�����Ϊһ����ɫ�������ӵ�֡������
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//�������data��������NULL,���������ڴ��û�����������Ⱦ��֡��������������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen_Width, Screen_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//�������ӵ���ǰ�󶨵�֡�������
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	/*
	glFrameBufferTexture2D�����µĲ�����
		target��֡�����Ŀ�꣨���ơ���ȡ�������߽��У�
		attachment��������Ҫ���ӵĸ������͡���ǰ�������ڸ���һ����ɫ������ע������0��ζ�����ǿ��Ը��Ӷ����ɫ���������ǽ���֮��Ľ̳����ᵽ��
		textarget����ϣ�����ӵ���������
		texture��Ҫ���ӵ�������
		level���༶��Զ����ļ������ǽ�������Ϊ0��
	*/

	/*
	������Ⱦ�������ͨ������ֻд�ģ����ǻᾭ��������Ⱥ�ģ�帽����
	��Ϊ�󲿷�ʱ�����Ƕ�����Ҫ����Ⱥ�ģ�建���ж�ȡֵ��ֻ������Ⱥ�ģ����ԡ�
	������Ҫ��Ⱥ�ģ��ֵ���ڲ��ԣ�������Ҫ�����ǽ��в�����������Ⱦ�������ǳ��ʺ����ǡ�
	�����ǲ���Ҫ����Щ�����в�����ʱ��ͨ������ѡ����Ⱦ���������Ϊ�������һ�㡣

	��Ⱦ��������ŵ㣺
	���Ὣ���ݴ���ΪOpenGLԭ������Ⱦ��ʽ������Ϊ������Ⱦ��֡�����Ż����ģ�
	��Ⱦ�������ֱ�ӽ����е���Ⱦ���ݴ��浽���Ļ����У��������κ���������ʽ��ת����������Ϊһ������Ŀ�д������ʣ�
	��Ϊ���������Ѿ���ԭ���ĸ�ʽ�ˣ���д����߸����������ݵ�����������ʱ�Ƿǳ���ġ����ԣ��������������Ĳ�����ʹ����Ⱦ�������ʱ��ǳ��죬
	������ÿ����Ⱦ�������ʹ�õ�glfwSwapBuffers��Ҳ����ͨ����Ⱦ�������ʵ�֣�ֻ��Ҫд��һ����Ⱦ����ͼ�񣬲�����󽻻�������һ����Ⱦ����Ϳ����ˡ�

	ͨ���Ĺ����ǣ�����㲻��Ҫ��һ�������в������ݣ���ô���������ʹ����Ⱦ�������������ǵ�ѡ��
	�������Ҫ�ӻ����в�����ɫ�����ֵ�����ݣ���ô��Ӧ��ѡ�������������ܷ�������������ǳ����Ӱ��ġ�
	*/

	//����һ����Ⱦ������󣬰�����Ȼ����ģ�建�壬���ǲ���Ҫ��������������
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//����һ����Ⱥ�ģ����Ⱦ�������GL_DEPTH24_STENCIL8��Ϊ�ڲ���ʽ������װ��24λ����Ⱥ�8λ��ģ�建��
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Screen_Width, Screen_Height); 
	//���������Ⱦ�������
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 
					
	//���֡�����Ƿ���������ʱ�Ѿ�������֡���岢������˸���
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	//���֡���壬��֤���ǲ��᲻С����Ⱦ�������֡������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window);

		//�󶨵�֡�����������Ƴ�������������ͨ������ɫ����һ��
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1, 0.1f, 0.1f ,1.0f);
		//����δʹ��ģ�建��
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MyShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Screen_Width / (float)Screen_Height, 0.1f, 100.0f);
		MyShader.setMat4("view", view);
		MyShader.setMat4("projection", projection);
		// ����
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
		// ����
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		MyShader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		/*
		���裺
		1.���µ�֡�����Ϊ�����֡���壬������һ����Ⱦ�������Ȱ����ж�����Ⱦ����
		2.��Ĭ�ϵ�֡����
		3.����һ�����������Ļ���ı��Σ���֡�������ɫ������Ϊ��������
		����������2��3
		*/

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		//������Ⱦ(Off-screen Rendering)��Ҫ��֤���е���Ⱦ�����������������Ӿ�Ч����������Ҫ�ٴμ���Ĭ��֡���壬�����󶨵�0
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//�ر���Ȳ��ԣ���Ϊ����֡�������ɵ�������һ����Ļ�ռ��µľ��Σ�����Ҫ��Ȳ���
		glDisable(GL_DEPTH_TEST); 
		
		//���������ػ�����
		//���������ɫ����Ϊ��ɫ��ʵ���ϲ����Ǳ�Ҫ�ģ���Ϊ�������ۻ����������ı��Σ������߿�ģʽ�¿��Կ�����
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		ScreenShader.use();
		glBindVertexArray(quadVAO);
		//ʹ����ɫ����������Ϊ�ı���ƽ�������
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &quadVBO);

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
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
