#include "shader_program.h"
#include <fstream>
#include <iostream>
#include<string>

using namespace std;

ShaderProgram::ShaderProgram(const char* vertexShaderPath,
    const char* fragmentShaderPath) {
    string vertexShaderString, fragmentShaderString;
    string buffer;

    ifstream vertexShaderFile(vertexShaderPath);
    ifstream fragmentShaderFile(fragmentShaderPath);

    while (getline(vertexShaderFile, buffer))
        vertexShaderString += buffer + '\n';

    vertexShaderFile.close();

    while (getline(fragmentShaderFile, buffer))
        fragmentShaderString += buffer + '\n';

    fragmentShaderFile.close();

    const char* vertexShaderCode = vertexShaderString.c_str();
    const char* fragmentShaderCode = fragmentShaderString.c_str();

    program = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
        error = true;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
        error = true;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
            << infoLog << std::endl;
        error = true;
    }
}

bool ShaderProgram::getError() { return error; }

GLuint ShaderProgram::getProgram() { return program; }
