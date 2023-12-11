/******************************************************************************|
| CPSC 4050/6050 Computer Garphics Assignment 5, Daljit Singh Dhillon, 2020    |
| Reference:                                                                   |
|                                                                              |
| Some OpenGL setup code here including math_funcs, and gl_utils               |
| are from Angton Gerdelan and "Anton's OpenGL 4 Tutorials."                   |
| http://antongerdelan.net/opengl/                                             |
| Email: anton at antongerdelan dot net                                        |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
|******************************************************************************/
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <vector>

#include "gl_utils.h"  // Anton's opengl functions and small utilities like logs
#include "maths_funcs.h"  // Anton's maths functions.
#include "stb_image.h"  // Sean Barrett's image loader with Anton's load_texture()

#define _USE_MATH_DEFINES
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0  // 0.017444444
#define TWO_PI 6.28318530718
void drawSurfaceOfRevolution();
mat4 view_mat;
mat4 proj_mat;
mat4 model_mat;
bool use_normal_map = false;
GLuint vbo, vao;
GLuint vertexShader, fragmentShader, shaderProgram;
int pointCount;
float evaluateParabolicArc(float t) { return t * t; }
void generateSurfaceOfRevolution(float* float_arr, int numYSteps,
                                 int numThetaSteps) {
  for (int i = 0; i < numYSteps; ++i) {
    float y = -1.0f + 2.0f * i / (numYSteps - 1);

    for (int j = 0; j < numThetaSteps; ++j) {
      float theta = static_cast<float>(j) / (numThetaSteps - 1) * TWO_PI;
      // Evaluate the base curve at the current y-coordinate

      // Use the evaluated point to construct the surface vertex in 3D space
      float r = evaluateParabolicArc(y);
      // Use the Y-component as the radial dimension
      float x = r * cos(theta);

      float z = r * sin(theta);

      float_arr[i * numThetaSteps + j] = x;
      float_arr[i * numThetaSteps + j + 1] = r;
      float_arr[i * numThetaSteps + j + 2] = z;
    }
  }
}
void loadSurfaceOfRevolution() {
  std::cout << "Enter the number of y steps" << std::endl;
  std::string buff;
  std::cin >> buff;
  int y_steps = atoi(buff.c_str());
  while (y_steps == 0) {
    std::cout << "error in input! input number of y steps again!" << std::endl;
    std::cin >> buff;
    y_steps = atoi(buff.c_str());
  }
  std::cout << "Enter the number of theta steps" << std::endl;
  std::cin >> buff;
  int theta_steps = atoi(buff.c_str());
  while (theta_steps == 0) {
    std::cout << "error in input! input number of theta steps again!"
              << std::endl;
    std::cin >> buff;
    theta_steps = atoi(buff.c_str());
  }
  float* float_arr = (float*)malloc(sizeof(float) * y_steps * theta_steps * 3);
  pointCount = y_steps * theta_steps;
  generateSurfaceOfRevolution(float_arr, y_steps, theta_steps);

  // VAO -- vertex attribute objects bundle the various things associated
  // with vertices
  glGenVertexArrays(
      1, &vao);            // generating and binding is common pattern in OpenGL
  glBindVertexArray(vao);  // basically setting up memory and associating it

  // VBO -- vertex buffer object to contain coordinates
  // MODIFY THE FOLLOWING BLOCK OF CODE APPRORIATELY FOR YOUR SURFACE OF
  // REVOLUTION

  // Compute and populate per-vertex normals

  // VAO -- vertex attribute objects bundle the various things associated with
  // vertices
  glGenVertexArrays(
      1, &vao);  // generating and binding is a common pattern in OpenGL
  glBindVertexArray(vao);  // basically setting up memory and associating it

  // VBO -- vertex buffer object to contain coordinates
  GLuint points_vbo;
  glGenBuffers(1, &points_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * y_steps * theta_steps * 3,
               float_arr, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  // Load textures

  // VBO -- normals -- needed for shading calcuations
  // ADD CODE TO POPULATE AND LOAD PER-VERTEX SURFACE NORMALS
  // [HINT] Vertex normals are organized in same order as that for vertex
  // coordinates

  // VBO -- vt -- texture coordinates
  // ADD CODE TO POPULATE AND LOAD PER-VERTEX TEXTURE COORDINATES
  // [HINT] texture coordinates are organized in same order as that for vertex
  // coordinates [HINT] there are two texture coordinates instead of three
  // vertex coordinates for each vertex
}

void loadUniforms(GLuint shader_programme) {
  /*---------------------------SET RENDERING
   * DEFAULTS---------------------------*/
  int model_mat_location = glGetUniformLocation(shader_programme, "model_mat");
  int view_mat_location = glGetUniformLocation(shader_programme, "view_mat");
  int proj_mat_location = glGetUniformLocation(shader_programme, "proj_mat");

  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
  glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat.m);
  glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat.m);

  // WRITE CODE TO LOAD OTHER UNIFORM VARIABLES LIKE FLAGS FOR ENABLING OR
  // DISABLING CERTAIN FUNCTIONALITIES
}

void drawSurfaceOfRevolution(std::vector<vec3>& surfaceVertices) {
  // MODIFY THIS LINE OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
  glDrawArrays(GL_TRIANGLES, 0, surfaceVertices.size());
}
void drawSurfaceOfRevolution() {
  // MODIFY THIS LINE OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
  glDrawArrays(GL_TRIANGLES, 0, pointCount);
}

void keyboardFunction(GLFWwindow* window, int key, int scancode, int action,
                      int mods) {
  // MODIFY THIS FUNCTION FOR KEYBOARD INTERACTIVITY
  // GLFW Reference Links:
  // Callback Example: https://www.glfw.org/docs/3.3/input_guide.html#input_key
  // List of Keys: https://www.glfw.org/docs/3.3/group__keys.html

  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    printf("\nKey 'E' pressed.... \n");
    // Example case. Key 'E' pressed. Doing nothing
  }

  if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
    // Close window when esacape is pressed
    glfwSetWindowShouldClose(g_window, 1);
  }
}
