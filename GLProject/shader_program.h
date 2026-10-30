#pragma once

#include "libs/glad_loader_docs.h"

class ShaderProgram {
public:
  ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
  bool getError();
  GLuint getProgram();

private:
  GLuint program;
  bool error = false;
};
