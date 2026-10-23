// module; // optional. Defines the beginning of the global module fragment

// #include directives go here but only apply to this file.
// import statements aren't allowed here. They go in the module preamble, below.

#pragma once

#include "../abstraction/OpenGlShape.hpp"
// import OpenGlShape;

class Drawer
{
public:
    void draw(OpenGlShape&);
    void draw(OpenGlShapeWithTexture&);
};