
#define GLM_ENABLE_EXPERIMENTAL 
#include "MyGlWindow.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


static float DEFAULT_VIEW_POINT[3] = { 5, 5, 5 }; 
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 }; 
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


MyGlWindow::MyGlWindow(int w, int h)

//==========================================================================
{
	m_width = w;
	m_height = h;
	m_cube = 0;

	glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 1.0f));

	model = translate * rotate * scale;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]); 
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]); 
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h); 
	m_viewer = std::make_unique<Viewer>(viewPoint, viewCenter, upVector, 60.0, aspect);

	initialize();
	setupBuffer();
	
}

void MyGlWindow::initialize()
{
	
	m_cube = std::make_unique<ColorCube>();
}

void MyGlWindow::setSize(int size_w, int size_h)
{
	m_width = size_w;
	m_height = size_h;
}
void MyGlWindow::setAspect(int size_aspect)
{
	m_aspect = size_aspect;
}

glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 z = glm::normalize(campos - look);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::normalize(glm::cross(z, x));

	glm::mat4 t = glm::translate(glm::mat4(1), -campos);
	glm::mat4 r({ x.x,y.x,z.x,0 }, { x.y,y.y,z.y,0 }, { x.z,y.z,z.z,0 }, { 0,0,0,1 });
	return r * t;
}

glm::mat4 Perspective(float fovy, float aspect, float p_near, float p_far)
{
	float f = 1.0f / glm::tan(glm::radians(fovy) / 2.0f);

	glm::mat4 m(0.0f);
	m[0][0] = f / aspect;
	m[1][1] = f;
	m[2][2] = (p_far + p_near) / (p_near - p_far);
	m[2][3] = -1.0f;
	m[3][2] = (2.0f * p_far * p_near) / (p_near - p_far);
	return m;
}


void MyGlWindow::setupBuffer()
{
	
	//쉐이더 객체 생성 : Morden cpp
	shaderProgram = std::make_unique<ShaderProgram>();
	
	//load shaders : vertex shader랑 fragment shader 지정
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	shaderProgram->addUniform("mvp");

}

void MyGlWindow::draw(void)
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2, 0.2, 0.2, 1); //백그라운드 색상 지정 (R, G, B ,A)
	//어떤 버퍼를 지울지 지정 : 라스터라이제이션 할때 두 개의 버퍼 : color, depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // | : bit or
	//0. 뷰포트 설정 : 캔버스 크기 및 위치 설정 : 현재는 800x800인데, 이걸 다 쓸건지 일부만 쓸건지 결정
	glViewport(0, 0, m_width, m_height); //앞 두개는 값: 시작점, 뒤 두개: 크기
	
	

	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();

	view = lookAt(eye, look, up);

	projection = Perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 500.0f);

	mvp = projection * view * model;

	
	
	//1. 쉐이더 use
	shaderProgram->use();
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	//2. vao binding
	if (m_cube) 
	{
		m_cube->draw();
	}
	//4. 쉐이더 disable
	
	shaderProgram->disable();
}

MyGlWindow::~MyGlWindow()
{

}
