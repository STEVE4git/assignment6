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
GLuint VAO, VBO;
GLuint vertexShader, fragmentShader, shaderProgram;
int pointCount;
float baseCurve(float t) {
  // Example: Parametric equation for an arc
  return 0.5 * cos(t);
}
void initialize(float* vertices, int numSegments, int numSlices) {
  // Create Vertex Array Object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Create Vertex Buffer Object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Generate and store the vertex data as a 1D float array

  for (int i = 0; i <= numSegments; ++i) {
    float t = (i / static_cast<float>(numSegments)) * 2.0f *
              M_PI;  // Angle in radians

    // Coordinates for the base curve
    float x = baseCurve(t);
    float y = 0.0;
    float z = 0.0;

    // Generate points on the surface of revolution
    for (int j = 0; j <= numSlices; ++j) {
      float theta = (j / static_cast<float>(numSlices)) * 2.0f *
                    M_PI;  // Angle in radians

      float newX = x * cos(theta);
      float newY = x * sin(theta);
      float newZ = z;

      // Store the coordinates in the 1D array
      vertices[(i * (numSlices + 1) + j) * 3] = newX;
      vertices[(i * (numSlices + 1) + j) * 3 + 1] = newY;
      vertices[(i * (numSlices + 1) + j) * 3 + 2] = newZ;
    }
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Specify the layout of the vertex data
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindVertexArray(0);
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
  float* float_arr =
      (float*)malloc(sizeof(float) * (3 * (y_steps + 1) * (theta_steps + 1)));
  pointCount = ((y_steps + 1) * (theta_steps + 1));
  initialize(float_arr, y_steps, theta_steps);

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
void drawSurfaceOfRevolution() {
  // MODIFY THIS LINE OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
  glDrawArrays(GL_TRIANGLE_STRIP, 0, pointCount);
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
