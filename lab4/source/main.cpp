#define GLM_ENABLE_EXPERIMENTAL 
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>

int main() {
	/*
	행렬의 곱셈 순서 (a * b != b * a)


	glm::mat3x3 a = { {1, 1, -1}, { 0, 2, 0 }, { 2, 3, 1 } };
	glm::mat3x3 b = { { 1, 0, 0 }, { 0, -1, 1 }, {0, 1, 2} };

	glm::mat3x3 m1 = a * b;
	glm::mat3x3 m2 = b * a;

	std::cout << "a * b : \n" << glm::to_string(m1) << std::endl;
	std::cout << "b * a : \n" << glm::to_string(m2) << std::endl;
	*/

	/* 
	행렬 a와 역행렬 invA의 곱은 단위행렬 I

	glm::mat2x2 A = { {4, 1}, {8, 3} };

	glm::mat2x2 invA = glm::inverse(A);

	glm::mat2x2 I = A * invA;

	std::cout << "I : \n" << glm::to_string(I) << std::endl;
	*/
	/*
	행렬 A와 벡터 B의 곱

	glm::mat3x3 A = { {1, 0, 0}, {2, -1, 0}, {3, 0, 1} };
	glm::vec3 B = { 1, 2, 3 };

	std::cout << "A * B : \n" << glm::to_string(A * B) << std::endl;
	std::cout << "B * A : \n" << glm::to_string(B * A) << std::endl;

	*/
	return 0;
}