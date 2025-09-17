#define GLM_ENABLE_EXPERIMENTAL 
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>

const double DEGREES_TO_RADIANS = glm::pi<float>() / 180.0;
const double RADIANS_TO_DEGREES = 180.0 / glm::pi<float>();

int main() {

	/* 벡터의 코사인 값 구하기
	
	glm::vec3 v(2, -1, 1);
	glm::vec3 u(1, 1, 2);

	float a = glm::dot(v, u); //벡터 v, c 내적
	float b = glm::length(v) * glm::length(u); //벡터 v, c 크기 곱

	float r = a / b; //코사인 값 구하기
	float angle = acos(r) * RADIANS_TO_DEGREES;	//코사인 값을 각도로 변환
	std::cout << "cos(v,u) = " << angle << std::endl;

	return 0;

	*/

	/* 벡터 분할
	* 
	glm::vec3 v(1, 2, -1);
	glm::vec3 u(0, 1, 0);

	glm::vec3 b = u * (glm::dot(v, u));
	glm::vec3 a = v - b;

	std::cout << "a: " << glm::to_string(a) << std::endl;
	std::cout << "b: " << glm::to_string(b) << std::endl;

	return 0;

	*/
	
	glm::vec3 n(0, 1, 0);
	glm::vec3 p(1, 0, 1);
	glm::vec3 x(-3, 3, 2);

	glm::vec3 v = x - p;

	n = glm::normalize(n);
	float d = glm::dot(v, n);

	std::cout << "d: " << d << std::endl;

	return 0;
}