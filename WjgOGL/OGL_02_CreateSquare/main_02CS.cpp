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
	//�ı�����߳��ϵ�ָ�����ڵ�������Ϊ��ǰ����
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initilize glad" << std::endl;
		return -1;
	}

	//���붥����ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int shaderSuccess;
	char shaderErrorLog[512];

	//��ⶥ����ɫ���Ƿ����ɹ�,GL_COMPILE_STATUS������״̬
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		//�洢����������Ϣ
		glGetShaderInfoLog(vertexShader, 512, NULL, shaderErrorLog);
		std::cout << "vertex shader compile error and log:" << shaderErrorLog << std::endl;
	}

	//����Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//���Ƭ����ɫ���Ƿ����ɹ�
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, shaderErrorLog);
		std::cout << "fragment shader compile error and log:" << shaderErrorLog << std::endl;
	}

	//��ɫ������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//���Ҫʹ�������Զ����������ɫ��������Ҫ���������ӵ�һ����ɫ������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//���������ɫ���ĳ����Ƿ�ʧ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, shaderErrorLog);
		std::cout << "shader program link error and log:" << shaderErrorLog << std::endl;
	}

	//жĥɱ¿
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//����󶨣�ע����˳��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//������VAO֮����EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
	/*��Ŀ����GL_ELEMENT_ARRAY_BUFFER��ʱ��VAO�ᴢ��glBindBuffer�ĺ������á�
	��Ҳ��ζ����Ҳ�ᴢ������ã�����ȷ����û���ڽ��VAO֮ǰ����������黺��(EBO)��
	��������û�����EBO�����ˡ�*/

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.6f, 0.5f ,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements�����ӵ�ǰ�󶨵�GL_ELEMENT_ARRAY_BUFFERĿ���EBO�л�ȡ����
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//����Ŀǰֻ��һ��VAO,���Բ���Ҫÿһ�ζ����
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	//ɾ����Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
