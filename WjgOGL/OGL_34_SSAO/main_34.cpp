#include "includes\glad\glad.h"
#include "includes\glfw\glfw3.h"
#include <iostream>
#include <random>

#include "includes\Shader.h"
#include "includes\camera.h"
#include "includes\model.h"

#include "includes\glm\glm.hpp"
#include "includes\glm\gtc\matrix_transform.hpp"
#include "includes\glm\gtc\type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "includes\stb_image.h"

#pragma comment ( lib,"glfw3.lib" ) 
#pragma comment ( lib,"assimp-vc140-mt.lib" ) 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void renderQuad();
void renderCube();

const unsigned int Screen_Width  = 1000;
const unsigned int Screen_Height = 800;

float lastX = Screen_Width/2.0f;
float lastY = Screen_Height/2.0f;

float deltaTime = 0.0f;		
float lastFrame = 0.0f;	

Camera camera(glm::vec3(2.0f, 2.0f, 5.0f));
bool firstMouse = true;

//һ����ֵ������������������������Ƭ��
float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

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

	//G����shader����Ҫ������������Ҫ�����ݴ浽G����
	Shader shaderGeometryPass("Shader/34/ssao_geometry.vs", "Shader/34/ssao_geometry.fs");
	//�������մ������Ǵ�G���嵱��������
	Shader shaderLightingPass("Shader/34/ssao.vs", "Shader/34/ssao_lighting.fs");
	//����ÿһ�����ɵ�Ƭ�μ����ڱ�ֵ(Ϊ�������յĹ�����ɫ����ʹ��)
	Shader shaderSSAO("Shader/34/ssao.vs", "Shader/34/ssao.fs");
	//ģ��ssao����
	Shader shaderSSAOBlur("Shader/34/ssao.vs", "Shader/34/ssao_blur.fs");

	// ����ģ��
	Model nanosuit("model/nanosuit.obj");

	//G����
	unsigned int gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	unsigned int gPosition, gNormal, gAlbedo;
	// λ����ɫ���壬���ǰ�λ����������rgb����������
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Screen_Width, Screen_Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// ������ɫ���壬�ѷ������ݴ���rgb����������
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Screen_Width, Screen_Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// ��ɫ���壬rgb����������Ŵ�������ͼ�����õ�����ɫ��a������Ÿ߹�ֵ(����ֵ)
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen_Width, Screen_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// ����openglʹ����һ����ɫ���� 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// ��Ȼ���
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Screen_Width, Screen_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//ssaoFBO�����һЩssao�׶εĽ������Ҫ�Ǵ�ż���õ��ڱ�ֵ
	//ssaoBlurFBO,������ģ�����
	unsigned int ssaoFBO, ssaoBlurFBO;
	glGenFramebuffers(1, &ssaoFBO);  
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	//SSAO��ɫ���帽��
	//���ڻ����ڱεĽ����һ���Ҷ�ֵ�����ǽ�ֻ��Ҫ����ĺ�ɫ�������������ǽ���ɫ������ڲ���ʽ����ΪGL_RED
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Screen_Width, Screen_Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
	// ģ��
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Screen_Width, Screen_Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//һ����λ�������ǿ��Ի��һ��ӵ�����64����ֵ�Ĳ�������
	//����0.0��1.0֮������������
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); 
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		// ����������������ԭ��ֲ�
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	/*
	���ǿ��ԶԳ�����ÿһ��Ƭ�δ���һ�������ת�����������ܿ콫�ڴ�ľ���
	���ԣ����õķ����Ǵ���һ��С�������ת��������ƽ������Ļ�ϡ�
	*/
	//һ��4x4�������߿ռ�ƽ�淨�ߵ������ת��������
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	//����һ�����������ת������4x4�����趨ΪGL_REPEAT���Ӷ���֤�����ʵ�ƽ������Ļ��
	unsigned int noiseTexture; 
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// �ƹ���Ϣ
	glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
	glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

	shaderLightingPass.use();
	shaderLightingPass.setInt("gPosition", 0);
	shaderLightingPass.setInt("gNormal", 1);
	shaderLightingPass.setInt("gAlbedo", 2);
	shaderLightingPass.setInt("ssao", 3);
	shaderSSAO.use();
	shaderSSAO.setInt("gPosition", 0);
	shaderSSAO.setInt("gNormal", 1);
	shaderSSAO.setInt("texNoise", 2);
	shaderSSAOBlur.use();
	shaderSSAOBlur.setInt("ssaoInput", 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window);

		glClearColor(0.1, 0.1f, 0.1f ,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// 1. ���δ���׶Σ������Ȱѳ�����Ⱦһ�Σ����Ұ�λ����������ɫ������������������ֵ�洢��G������
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Screen_Width / (float)Screen_Height, 0.1f, 50.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		shaderGeometryPass.use();
		shaderGeometryPass.setMat4("projection", projection);
		shaderGeometryPass.setMat4("view", view);
		// ��Ⱦ�󷽿鵱������
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		shaderGeometryPass.setMat4("model", model);
		//��ת���ߣ���Ϊ������cube���ڲ�
		shaderGeometryPass.setInt("invertedNormals", 1); 
		renderCube();
		shaderGeometryPass.setInt("invertedNormals", 0); 
		// ��Ⱦ���ϵĽ�ɫģ��
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shaderGeometryPass.setMat4("model", model);
		nanosuit.Draw(shaderGeometryPass);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 2.����SSAO����
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSSAO.use();
		// ���Ͳ�������
		for (unsigned int i = 0; i < 64; ++i)
			shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
		shaderSSAO.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 3. SSAO�׶κ͹��ս׶�֮�䣬����ģ��SSAO����Ĵ���
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSSAOBlur.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 4. ����pass: ��ͳ��Blinn-Phong�ӳٹ��գ�����������Ļ�ռ�Ļ������ڵ�
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderLightingPass.use();
		// ���͹�����ص�uniform
		glm::vec3 lightPosView = glm::vec3(camera.GetViewMatrix() * glm::vec4(lightPos, 1.0));
		shaderLightingPass.setVec3("light.Position", lightPosView);
		shaderLightingPass.setVec3("light.Color", lightColor);
		// ����˥��ϵ��
		const float constant  = 1.0; 
		const float linear    = 0.09;
		const float quadratic = 0.032;
		shaderLightingPass.setFloat("light.Linear", linear);
		shaderLightingPass.setFloat("light.Quadratic", quadratic);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedo);
		//Ϊ����pass��Ӷ����SSAO����
		glActiveTexture(GL_TEXTURE3); 
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		renderQuad();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
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
