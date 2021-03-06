
#define GLM_ENABLE_EXPERIMENTAL 1

#ifdef _WIN32
#define GLEW_STATIC 1
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

GLuint shader_programme;
GLuint vao;

std::string load_shader(const std::string& path)
{
  std::string line;
  std::string program;
  std::ifstream src(path);

  if (src.is_open())
    {
      while (std::getline(src, line))
	{
	  program += line;
	  program += "\n";
	}
      src.close();
    }
	
  return program;
}

std::vector<float> square(const float x, const float y, const float w, const float h) 
{
  std::vector<float> verts = {
    x,			y,
    (x - w),	y,
    x,			(y - h),
    x,			(y - h),
    (x - w),	(y),
    (x - w),	(y - h)
  };
  return verts;
}

void initialize()
{

  std::vector<float> vss = square(-0.1f, -0.1f, 0.2f, 0.2f);

  float* points = &vss[0];

  GLuint vbo = 0;

  //glm::vec4 pt(100, 100, 0, 0);
  //glm::vec4 tpt = ortho * pt;
  //std::cout << glm::to_string(tpt) << std::endl;


  // Insert n number of unused names to vbo
  glGenBuffers(1, &vbo);
  // Acquire buffer state (alternatively glCreateBuffer() to do both)
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Allocate mutable data store
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

  vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

  std::string vert = load_shader("../main.vert");
  std::string frag = load_shader("../main.frag");

  const char* vertex_shader = vert.c_str();
  const char* fragment_shader = frag.c_str();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  GLuint mvp_handle = glGetUniformLocation(shader_programme, "MVP");
  glm::mat4 projection = glm::ortho(0.f, 400.f, 0.f, 400.f, 0.f, 100.f);
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 mvp = projection * model;
  glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, &mvp[0][0]);
}

void render()
{
  glClearColor(255, 255, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader_programme);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);


  //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  window = glfwCreateWindow(640, 480, "Sunstone 2D Demo", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    exit(EXIT_FAILURE);
  }

  char* version = (char*)glGetString(GL_VERSION);

  std::cout << "OpenGL version: " << version << std::endl;
  
  initialize();

  glfwSetKeyCallback(window, key_callback);
  
  while (!glfwWindowShouldClose(window))
    {
      
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
		        
      render();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
