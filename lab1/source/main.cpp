#define GLM_ENABLE_EXPERIMENTAL 
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
//using namespace glm; 사용 잘 안함

/*

//glm 은 객체 지향이 아니라 c와 비슷하다
int main()
{
	glm::vec2 a(1.0f, 4.0f);
	//2차원 벡터의 길이?
	// a.length(); -> 차원을 반환해주는 것 (2차원, 3차원...)
	//백터의 길이를 구하는 공식 / 길이는 스칼라여서 무조건 float로 받아줘야함
	float l = glm::length(a);

	glm::vec3 b(1.0f, 4.0f, 2.0f);

	//b = b * 3.0 <- 오류, b는 float고 3.0은 double이라서

	glm::vec3 c = glm::vec3(1, 1, 1);

	//glm::normalize(a); << 이거 normalize만 사용하면 안됨, 받는게 없음 -> 중간고사 때 틀리기 쉬움
	//d = glm::normalize(c); 반환값을 받아줘야함
	glm::vec3 d = glm::normalize(c);

	c.xyz = d.xyz; //c.x = d.x; c.y = d.y; c.z = d.z;

	std::cout << glm::to_string(a) << std::endl;
	

	//glm::normalize(a); << 이거 normalize만 사용하면 안됨, 받는게 없음 -> 중간고사 때 틀리기 쉬움
	c = glm::normalize(c); //반환값을 받아줘야함



	return 0;
}
*/

int main() {
	//Q1
	glm::vec3 p(1.0f, 4.0f, -5.0f);
	glm::vec3 a(5.0f, 1.0f, 4.0);

	std::cout << glm::to_string(p + a) << std::endl;

	//Q2
	glm::vec3 c(1.0f, 0.0f, 0.0f);
	glm::vec3 d(0.0f, 1.0f, 0.0f);

	std::cout << glm::to_string(c + d) << std::endl;

	//Q3
	glm::vec3 e(0.0f, 1.0f, 0.0f);
	std::cout << glm::to_string(e * 2.0f) << std::endl;

	//Q4
	glm::vec3 f(8.0f, 2.0f, 5.0f);
	glm::vec3 g(1.0f, -1.0f, 4.0f);

	std::cout << glm::to_string(g - f) << std::endl;

	//Q5
	glm::vec3 h(1.0f, 3.0f, 4.0f);
	h = glm::normalize(h);
	std::cout << glm::to_string(h) << std::endl;

	//Q6
	glm::vec3 hh(1.0f, -1.0f, 4.0f);
	glm::vec4 i(hh, 0.0f);
	std::cout << glm::to_string(i) << std::endl;
}