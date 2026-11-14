#define LECTURE_TITLE "Hello Textured Model"
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

#include "libs/glad_loader_docs.h"
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "model.h"
#include "shader_program.h"
#include "textured_model.h"

using namespace std;
using namespace glm;
using namespace Assimp;

GLuint generateTexture(const char *imagePath) {
  // Load image file
  int width, height, nrChannels;
  unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

  GLuint texture;

  glGenTextures(1, &texture);

  // Set as current
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set wrapping modes along u (s) and v (t)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Set minimize and maximize filters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image data into VRAM
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);

  return texture;
}

TexturedModel loadModelFromFile(const char *meshPath) {
  Importer importer;

  // Load the model file
  // Note that we are triangulating it here with aiProcess_Triangulate flag
  // So that we are sure everything is composed of triangles
  const aiScene *scene = importer.ReadFile(meshPath, aiProcess_Triangulate);

  // We want the first mesh (group of vertices) in the model file
  aiMesh *mesh = scene->mMeshes[0];

  vector<vec3> positions(mesh->mNumFaces * 3);
  vector<vec2> uvs(mesh->mNumFaces * 3);

  // Now we iterate over the faces of the mesh
  for (int i = 0; i < mesh->mNumFaces; ++i) {
    // For each face
    for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {

      // We obtain the index of the vertex and grab its position and uv data
      auto position = mesh->mVertices[mesh->mFaces[i].mIndices[j]];
      auto uv = mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]];

      positions.push_back({position.x, position.y, position.z});
      uvs.push_back({uv.x, uv.y});
    }
  }

  return TexturedModel(positions, uvs);
}

// Gets called whenever the application window is resized
void windowResizeCallback(GLFWwindow *window, int width, int height) {
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

  // Set the window resize callback
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

  vector<vec2> uvs;

  // First face
  uvs.push_back({1, 1});
  uvs.push_back({0, 0});
  uvs.push_back({0, 1});

  // Second face
  uvs.push_back({1, 1});
  uvs.push_back({1, 0});
  uvs.push_back({0, 0});

  // Third face
  uvs.push_back({1, 0});
  uvs.push_back({0, 1});
  uvs.push_back({0, 0});

  // Fourth face
  uvs.push_back({1, 1});
  uvs.push_back({0, 1});
  uvs.push_back({0, 0});

  TexturedModel tetrahedron(positions, uvs);

  // We load this model from file
  // The file path is passed as a parameter
  TexturedModel sphere = loadModelFromFile("sphere.glb");

  if (GLenum err = glGetError() != 0) {
    return err;
  }

  // We load two textures into the VRAM
  GLuint texture = generateTexture("./texture.png");
  GLuint texture2 = generateTexture("./texture2.png");

  if (GLenum err = glGetError() != 0) {
    return err;
  }

  // Load and link shader files
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

  // Store the time of the last frame, this will be the first frame since it is
  // outside the for loop
  double lastFrameTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    double thisFrameTime = glfwGetTime();

    // Calculate the time passed between both frames
    double deltaTime = thisFrameTime - lastFrameTime;
    lastFrameTime = thisFrameTime;

    // Add the change to the rotation angle
    angle += deltaTime;

    mat4 rotation = rotate(mat4(1.0f), angle, {0, 1, 0});
    mat4 translation = translate(mat4(1.0f), {0, 0, -7});

    // We first apply the rotation (around the center)
    // Followed by a translation
    mat4 modelMatrix = translation * rotation;

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    // Note that now we ALSO clear the depth values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the current texture
    glBindTexture(GL_TEXTURE_2D, texture);
    tetrahedron.draw();

    // We update the model matrix so our models do not overlap
    modelMatrix = translate(mat4(1.0f), {2, 0, -7});
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    // Switch the current texture as well
    glBindTexture(GL_TEXTURE_2D, texture2);
    sphere.draw();

    // Draw the final pixels/image on the created window
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Free resources
  glfwTerminate();
  return 0;
}
