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

#include "gl_utils.h"  // Anton's opengl functions and small utilities like logs
#include "maths_funcs.h"  // Anton's maths functions.
#include "stb_image.h"  // Sean Barrett's image loader with Anton's load_texture()

#define _USE_MATH_DEFINES
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0  // 0.017444444

mat4 view_mat;
mat4 proj_mat;
mat4 model_mat;
GLuint vao, vbo, ebo;
int pointCount;
GLuint normals_vbo;
#include <vector>

std::vector<std::pair<float, float>> generateParabolicCurve(int ySteps) {
  std::vector<std::pair<float, float>> curvePoints;

  for (int i = 0; i <= ySteps; ++i) {
    float y = -1.0f + (2.0f * i / ySteps);  // Normalize y within [-1, 1]
    float r = y * y;                        // Parabolic curve formula

    curvePoints.push_back(std::make_pair(y, r));
  }

  return curvePoints;
}
void generateSurfaceOfRevolution(int ySteps, int thetaSteps) {
  std::vector<std::pair<float, float>> baseCurve =
      generateParabolicCurve(ySteps);

  // Array of points pij
  std::vector<GLfloat> points;
  std::vector<GLfloat> normals;

  std::vector<GLfloat> texCoords;
  for (int j = 0; j < thetaSteps; ++j) {
    float theta = (2.0f * M_PI * j) / thetaSteps;

    for (int i = 0; i <= ySteps; ++i) {
      float y = baseCurve[i].first;
      float r = baseCurve[i].second;
      float x = r * cos(theta);
      float z = r * sin(theta);
      if (z < 0) continue;

      vec3 normal = normalise(vec3(x, y, z));
      float u = static_cast<float>(j) / (thetaSteps - 1);
      float v = 1.0f - static_cast<float>(i) / ySteps;

      texCoords.push_back(u);
      texCoords.push_back(v);

      // Add the normal to the normals vector
      normals.push_back(normal.v[0]);
      normals.push_back(normal.v[1]);
      normals.push_back(normal.v[2]);

      points.push_back(x);
      points.push_back(y);
      points.push_back(z);
    }
  }

  // List of quads (break into triangles)
  std::vector<GLuint> indices;

  for (int j = 0; j < thetaSteps - 1; ++j) {
    for (int i = 0; i < ySteps; ++i) {
      int p0 = j * (ySteps + 1) + i;
      int p1 = p0 + 1;
      int p2 = (j + 1) * (ySteps + 1) + i;
      int p3 = p2 + 1;

      // First triangle
      indices.push_back(p0);
      indices.push_back(p1);
      indices.push_back(p2);

      // Second triangle
      indices.push_back(p1);
      indices.push_back(p3);
      indices.push_back(p2);
    }
  }
  pointCount = indices.size();
  // Now organize the points and indices into VBO and VAO
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glGenBuffers(1, &normals_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat),
               normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(1);
  GLuint texCoords_vbo;
  glGenBuffers(1, &texCoords_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
  glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat),
               texCoords.data(), GL_STATIC_DRAW);

  // Vertex attribute pointers for texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(2);
  for (int i = 0; i < texCoords.size(); ++i)
    std::cout << texCoords[i] << std::endl;
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

  generateSurfaceOfRevolution(y_steps, theta_steps);
  glBindTexture(GL_TEXTURE_2D, tex00);
  /*------------------------------CREATE
   * GEOMETRY-------------------------------*/

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

  // Choose vertex and fragment shaders to use as well as view and proj
  // matrices.

  int model_mat_location = glGetUniformLocation(shader_programme, "model_mat");
  int view_mat_location = glGetUniformLocation(shader_programme, "view_mat");
  int proj_mat_location = glGetUniformLocation(shader_programme, "proj_mat");

  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
  glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat.m);
  glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_mat.m);
  // Set light and view positions
  glUniform3f(glGetUniformLocation(shader_programme, "lightPos"), cam_pos.v[0],
              cam_pos.v[1], cam_pos.v[2]);
  glUniform3f(glGetUniformLocation(shader_programme, "viewPos"), cam_pos.v[0],
              cam_pos.v[1], cam_pos.v[2]);

  // Set other parameters (e.g., ambientStrength, specularStrength, shininess)
  glUniform1f(glGetUniformLocation(shader_programme, "ambientStrength"), 0.1);
  glUniform1f(glGetUniformLocation(shader_programme, "specularStrength"), 0.5);
  glUniform1f(glGetUniformLocation(shader_programme, "shininess"), 32.0);
  glUniform3f(glGetUniformLocation(shader_programme, "lightColor"), 3.0f, 2.0f,
              3.0f);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex00);
  glUniform1i(glGetUniformLocation(shader_programme, "texture00"), 0);
  // lUniform1i(glGetUniformLocation(shader_programme, "texture00"), 0);
  //  WRITE CODE TO LOAD OTHER UNIFORM VARIABLES LIKE FLAGS FOR
  //  ENABLING OR DISABLING CERTAIN FUNCTIONALITIES
}

void drawSurfaceOfRevolution() {
  glBindVertexArray(vao);
  glBindVertexArray(0);
  glBindVertexArray(1);
  glBindVertexArray(2);
  glBindVertexArray(vbo);
  glBindVertexArray(ebo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex00);
  glDrawElements(GL_TRIANGLES, pointCount, GL_UNSIGNED_INT, 0);
  glDrawElements(GL_TRIANGLES, pointCount, GL_UNSIGNED_INT, (void*)1);
  glDrawElements(GL_TRIANGLES, pointCount, GL_UNSIGNED_INT, (void*)2);
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
