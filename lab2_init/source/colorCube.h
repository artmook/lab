#include <iostream>
#include <GL/gl3w.h>
#include <string>
#include "Loader.h"

class ColorCube
{
private:
	GLuint vaohandle;
	GLuint ibo_cube_elements;
	GLuint vbo_position;
	GLuint vbo_color;
	void setup();
	
public:
	ColorCube();
	~ColorCube();
	void draw();
};