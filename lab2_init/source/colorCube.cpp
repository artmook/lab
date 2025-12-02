
#define GLM_ENABLE_EXPERIMENTAL 
#include "colorCube.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>



void ColorCube::setup()
{
	
	GLfloat cube_vertices[] = {
		// front
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,
		-1.0, 1.0, -1.0,
	};
	GLfloat cube_colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors 
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
	};
	GLushort cube_elements[] = {
		0, 1, 2,	2, 3, 0,	1, 5, 6,
		6, 2, 1,	7, 6, 5,	5, 4, 7,
		4, 0, 3,	3, 7, 4,	4, 5, 1,
		1, 0, 4,	3, 2, 6,	6, 7, 3,
	};
	glCreateVertexArrays(1, &vaohandle);
	glCreateBuffers(1, &vbo_position);
	glCreateBuffers(1, &vbo_color);
	//2.vbo 생성
	//position 처리
	//GPU 메모리 할당
	glNamedBufferData(vbo_position, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//GPU에게 해석방법 알려줌
	glVertexArrayVertexBuffer(
		vaohandle, //vao
		0,//binding index 번호 : vao 컨테이너 안 16개의 vbo슬롯 중 0번
		vbo_position,//vbo
		0,//offset
		sizeof(float) * 3 //stride
	);

	//color 처리
	//GPU 메모리 할당
	glNamedBufferData(vbo_color, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	//GPU에게 해석방법 알려줌
	glVertexArrayVertexBuffer(
		vaohandle, //vao
		1,//binding index 번호 : vao 컨테이너 안 16개의 vbo슬롯 중 1번
		vbo_color,//vbo
		0,//offset
		sizeof(float) * 3 //stride
	);

	glVertexArrayAttribFormat(
		vaohandle,
		0, //속성번호
		3, //vertex당 데이타 수
		GL_FLOAT,
		GL_FALSE,
		0//offset
	);
	glVertexArrayAttribBinding(vaohandle,
		0, //속성번호 0
		0 //binding index 0
	);
	glEnableVertexArrayAttrib(vaohandle, 0); //0번 속성 enable

	glVertexArrayAttribFormat(
		vaohandle,
		1, //속성번호
		3, //vertex당 데이타 수
		GL_FLOAT,
		GL_FALSE,
		0//offset
	);
	glVertexArrayAttribBinding(vaohandle,
		1, //속성번호 1
		1 //binding index 1
	);
	glEnableVertexArrayAttrib(vaohandle, 1); //1번 속성 enable

	glCreateBuffers(1, &ibo_cube_elements); //Gluint ibo_cube_elements
	glNamedBufferData(ibo_cube_elements, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaohandle, ibo_cube_elements);
	
	
}

ColorCube::ColorCube()
{
	setup();
}

void ColorCube::draw()
{
	glBindVertexArray(vaohandle);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


}

ColorCube::~ColorCube()
{

}