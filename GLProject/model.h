#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "libs/glad_loader_docs.h"
using namespace glm;
using namespace std;

class Model {
public:
  Model(vector<vec3> postions, vector<vec3> colors);
  void draw();

private:
  GLuint VAO;
  unsigned long verticesCount;
};
