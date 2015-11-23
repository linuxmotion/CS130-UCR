#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

string Read_Shader(string shader_name);
GLuint Load_Shader(string shader_name, GLenum shader_type);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", 0, 0);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	//Initialize vertex data
	GLfloat vertices[] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f };

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vertex_shader = Load_Shader("VertexShader.glsl",GL_VERTEX_SHADER);
	GLuint fragment_shader = Load_Shader("FragmentShader.glsl", GL_FRAGMENT_SHADER);
	GLuint tess_control_shader = Load_Shader("TessControlShader.glsl", GL_TESS_CONTROL_SHADER);
	GLuint tess_eval_shader = Load_Shader("TessEvalShader.glsl", GL_TESS_EVALUATION_SHADER);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glAttachShader(shaderProgram, tess_control_shader);
	glAttachShader(shaderProgram, tess_eval_shader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	//Link vertex position data to the shader position variable
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	//Initialize uniforms in the tessellation control shader
	GLint TessLevelInner = glGetUniformLocation(shaderProgram, "TessLevelInner");
	GLint TessLevelOuter = glGetUniformLocation(shaderProgram, "TessLevelOuter");

	//Parameters used in tessellation shader
	glUniform1f(TessLevelInner, 1);
	glUniform1f(TessLevelOuter, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glPatchParameteri(GL_PATCH_VERTICES, 3);       // tell OpenGL that every patch has 3 verts
		glDrawArrays(GL_PATCHES, 0, 3);					//Draw vertices
	}

	glfwTerminate();

	return 0;
}

string Read_Shader(string shader_name)
{
	string shader_code;
	ifstream file_input(shader_name.c_str());
	if (file_input.is_open()) {
		stringstream stream_input;
		stream_input << file_input.rdbuf();
		shader_code = stream_input.str();
	}
	else
		cerr << "Error " <<  shader_name << " missing!" << endl;

	return shader_code;
}

GLuint Load_Shader(string shader_name, GLenum shader_type)
{
	string shader_code = Read_Shader(shader_name);
	const GLchar* shader_source = shader_code.c_str();

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	GLint compile_status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	char compile_status_buffer[512];
	glGetShaderInfoLog(shader, 512, NULL, compile_status_buffer);
	cerr << compile_status_buffer << endl;

	return shader;
}

