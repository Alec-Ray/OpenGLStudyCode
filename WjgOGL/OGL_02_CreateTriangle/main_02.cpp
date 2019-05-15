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

	//жĥɱ¿��������ɫ�������Ѿ����ӵ�һ����ɫ���������ˣ����ǲ���Ҫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��������,NDC����ϵ�µ�λ��
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	//���㻺�����,���㻺�����Ļ���������GL_ARRAY_BUFFER
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//�����������
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	/*VAO����û�д洢���������������ݣ���Щ��Ϣ�Ǵ洢��VBO�еģ�
	VAO�൱���ǶԺܶ��VBO�����ã���һЩVBO�����һ����Ϊһ������ͳһ����
	ִ��VAO��֮����������VBO���ö������VAO�����һ���֣�
	����˵VBO�ǶԶ���������Ϣ�İ󶨣�VAO�ǶԺܶ��VBO�İ󶨡�
	*/
	//��VAO
	glBindVertexArray(VAO);

	//��VBO,����֮��ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�Ѷ���õĶ������ݸ��Ƶ�������ڴ���
	/*�����ε�λ�����ݲ���ı䣬ÿ����Ⱦ����ʱ������ԭ������������ʹ�����������GL_STATIC_DRAW��
	���������˵һ�������е����ݽ�Ƶ�����ı䣬��ôʹ�õ����;���GL_DYNAMIC_DRAW��GL_STREAM_DRAW��
	��������ȷ���Կ������ݷ����ܹ�����д����ڴ沿�֡�*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*	
	���磺layout( location=index ) in vec4 position;
	������index��glVertexAttribPointer�ĵ�һ������һ����
	��ô��ػ����������ݾͻᴫ�ݵ����position������ȥ��
	glVertexAttribPointerִ�к󣬻�Ӱ��ı�VAO��״̬��VBO�ᱻ���Ʊ��浽VAO�С�
	֮������ı��˵�ǰ���󶨵Ļ������Ҳ����ı䵽VAO��Ķ���
	*/
	//���ö�������ָ��,��һ��0���Ƕ�����ɫ������д�Ķ�������λ��ֵ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//�Զ�������λ��ֵ0[������ɫ������Ҳд��:layout(location=0)]��Ϊ���������ö�������
	glEnableVertexAttribArray(0);

	//���VBO��VAO�İ󶨣�ע����˳��
	//֮���Կ��Խ��VBO������Ϊִ��glVertexAttribPointer��OpenGL�Ѿ��ҵ���VBO�Ķ�������λ��ֵ
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//���VAO�󶨷�ֹ���VAO��������VAO�ĵ����޸�,����ʵ������ٷ���
	glBindVertexArray(0);


	//------------------------------------------------------------------------------------------------
	/*
	ǰ������һ�ж��ǵȴ���һ�̣�һ�����������Ƕ����������ú�Ӧʹ�õ�VBO�Ķ����������
	һ�㵱�������ƶ������ʱ��������Ҫ����/�������е�VAO���ͱ����VBO������ָ��)��
	Ȼ�󴢴����ǹ�����ʹ�á������Ǵ�����������ʱ����ó���Ӧ��VAO��
	������������������ٽ��VAO��
	*/
	//------------------------------------------------------------------------------------------------


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.6f, 0.5f ,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//����Ŀǰֻ��һ��VAO,���Բ���Ҫÿһ�ζ����
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
