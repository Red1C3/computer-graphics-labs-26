#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "libs/glad_loader_docs.h"

using namespace glm;
using namespace std;

class TexturedModel {
public:
  TexturedModel(vector<vec3> postions, vector<vec2> uvs, vector<vec3> normals);
  void draw();

private:
  GLuint VAO;
  unsigned long verticesCount;
};