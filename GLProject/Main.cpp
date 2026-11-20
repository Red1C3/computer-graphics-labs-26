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

#include "shader_program.h"
#include "textured_model.h"

using namespace std;
using namespace glm;
using namespace Assimp;

GLuint generateTexture(const char* imagePath) {
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image data into VRAM
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);

  return texture;
}

TexturedModel loadModelFromFile(const char* meshPath) {
  Importer importer;

  const aiScene* scene = importer.ReadFile(meshPath, aiProcess_Triangulate);

  aiMesh* mesh = scene->mMeshes[0];

  vector<vec3> positions(mesh->mNumFaces * 3);
  vector<vec2> uvs(mesh->mNumFaces * 3);
  vector<vec3> normals(mesh->mNumFaces * 3); // Read and parse normals as well

  for (int i = 0; i < mesh->mNumFaces; ++i) {
      for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
          auto position = mesh->mVertices[mesh->mFaces[i].mIndices[j]];
          auto uv = mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]];
          auto normal = mesh->mNormals[mesh->mFaces[i].mIndices[j]];

          positions.push_back({ position.x, position.y, position.z });
          uvs.push_back({ uv.x, uv.y });
          normals.push_back({ normal.x, normal.y, normal.z });
      }
  }

  return TexturedModel(positions, uvs, normals);
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

  // Enable blending and set the blending function
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

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
  GLuint transparencyLocation =
    glGetUniformLocation(shaderProgram.getProgram(), "transparent");

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
    
    // Note that now we ALSO clear the depth values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    angle += deltaTime;
    
    mat4 rotation = rotate(mat4(1.0f), angle, { 0, 1, 0 });
    mat4 translation = translate(mat4(1.0f), { 0, 0, -7 });

    // We first apply the rotation (around the center)
    // Followed by a translation
    mat4 modelMatrix = translation * rotation;

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform1i(transparencyLocation, 0); // Not transparent

    glBindTexture(GL_TEXTURE_2D, texture);
    sphere.draw();

    modelMatrix = translate(mat4(1.0f), { 1, -1, -5 });

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform1i(transparencyLocation, 1); //Transparent
    sphere.draw();

    // Draw the final pixels/image on the created window
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Free resources
  glfwTerminate();
  return 0;
}
