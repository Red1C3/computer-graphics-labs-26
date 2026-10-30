#define LECTURE_TITLE "Hello Tetrahedron"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "model.h"
#include "shader_program.h"

using namespace std;
using namespace glm;

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


int main(int argc, char *argv[]) {
  if (argc > 0) {
    std::filesystem::path exePath =
        std::filesystem::canonical(argv[0]).parent_path();
    std::filesystem::current_path(exePath);
  }
  // Initialize GLFW library and create window to draw on it
  // GLFW manages the window
  glfwInit();

  // Set OpenGL Version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Specify depth buffer bits as well so we can make use of depth features
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, LECTURE_TITLE, NULL, NULL);
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, windowResizeCallback);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  // Enable depth
  glEnable(GL_DEPTH_TEST);
  vector<vec3> positions = {};

  // First face
  positions.push_back({1, 1, 1});
  positions.push_back({1, -1, -1});
  positions.push_back({-1, 1, -1});

  // Second face
  positions.push_back({1, 1, 1});
  positions.push_back({1, -1, -1});
  positions.push_back({-1, -1, 1});

  // Third face
  positions.push_back({1, -1, -1});
  positions.push_back({-1, 1, -1});
  positions.push_back({-1, -1, 1});

  // Fourth face
  positions.push_back({1, 1, 1});
  positions.push_back({-1, 1, -1});
  positions.push_back({-1, -1, 1});

  vector<vec3> colors = {};

  // First face
  for (int i = 0; i < 3; ++i) {
    colors.push_back({1, 1, 1});
  }

  // Second face
  for (int i = 0; i < 3; ++i) {
    colors.push_back({1, 0, 0});
  }

  // Third face
  for (int i = 0; i < 3; ++i) {
    colors.push_back({0, 1, 0});
  }

  // Forth face
  for (int i = 0; i < 3; ++i) {
    colors.push_back({0, 0, 1});
  }

  Model tetrahedron(positions, colors);
  if (GLenum err = glGetError() != 0) {
    return err;
  }
  ShaderProgram shaderProgram("shaders/shader.vert", "shaders/shader.frag");
  if (shaderProgram.getError()) {
    return -1;
  }

  if (GLenum err = glGetError() != 0) {
    return err;
  }

  // Each uniform has a location (think of it as an ID)
  // Before we update the uniform we need to get its location
  GLuint modelMatrixLocation =
      glGetUniformLocation(shaderProgram.getProgram(), "model");
  GLuint viewMatrixLocation =
      glGetUniformLocation(shaderProgram.getProgram(), "view");
  GLuint perspectiveMatrixLocation =
      glGetUniformLocation(shaderProgram.getProgram(), "perspective");

  if (GLenum err = glGetError() != 0) {
    return err;
  }

  // Note that we cannot update the uniform without setting the shader program
  // first
  glUseProgram(shaderProgram.getProgram());

  vec3 cameraPosition{0, 0, 0};
  vec3 lookedAt{0, 0, -1};
  vec3 up{0, 1, 0};

  // View matrix controls the camera/observer
  mat4 viewMatrix = lookAt(cameraPosition, lookedAt, up);

  // We send the pointer to the first element of the matrix (notice the [0][0])
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

  float fieldOfView = 45.0f;
  float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
  float nearPlane = 0.01f;
  float farPlane = 100.0f;

  mat4 perspectiveMatrix =
      perspective(fieldOfView, aspectRatio, nearPlane, farPlane);

  glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_FALSE,
                     &perspectiveMatrix[0][0]);

  if (GLenum err = glGetError() != 0) {
    return err;
  }

  // We'll use this angle to rotate the tetrahedron
  float angle = 0.f;

  double lastFrameTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    double thisFrameTime = glfwGetTime();
    double deltaTime = thisFrameTime - lastFrameTime;
    lastFrameTime = thisFrameTime;

    angle += deltaTime;

    mat4 rotation = rotate(mat4(1.0f), angle, {0, 1, 0});
    mat4 translation = translate(mat4(1.0f), {0, 0, -5});

    // We first apply the rotation (around the center)
    // Followed by a translation
    mat4 modelMatrix = translation * rotation;

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    // Note that now we ALSO clear the depth values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    tetrahedron.draw();
    // Draw the final pixels/image on the created window
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Free resources
  glfwTerminate();
  return 0;
}
