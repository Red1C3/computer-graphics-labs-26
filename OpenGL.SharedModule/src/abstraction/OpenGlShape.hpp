// module; // optional. Defines the beginning of the global module fragment

// #include directives go here but only apply to this file.
// import statements aren't allowed here. They go in the module preamble, below.
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


using namespace std;

// TODO, accept VBO in ctor!
class OpenGlShape
{
protected:
	OpenGlShape(); // using empty ctor is not allowed to be exposed
	GLenum drawing_mode_;
	unsigned int number_of_vertices_;
	unsigned int VBO_, VAO_;

public:
	OpenGlShape(const vector<glm::vec3>&, GLenum);
	OpenGlShape(const vector<float>&, GLenum);
	~OpenGlShape();
	unsigned int get_vao();
	GLenum get_drawing_mode();
	int get_number_of_vertices();
};

// TODO, accept passing VBO in ctor!
class OpenGlShapeWithTexture : public OpenGlShape
{
private:
	unsigned int texture_id_;

public:

	OpenGlShapeWithTexture(const vector<glm::vec3>&, GLenum,
		const char*, const vector<glm::vec2>&);
	OpenGlShapeWithTexture(const vector<float>& vertices, GLenum mode,
	const char* texture_path);
	~OpenGlShapeWithTexture();
	unsigned int get_texture_id();
};


class OpenGlShapeWithColor : public OpenGlShape
{
private:
	unsigned int texture_id_;

public:
	OpenGlShapeWithColor(const vector<glm::vec3>&, GLenum);
	OpenGlShapeWithColor(const vector<float>&, GLenum);
	~OpenGlShapeWithColor();
	unsigned int get_texture_id();
};