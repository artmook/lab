#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat3x3.hpp>

int main() {
	/*
	
	동차 좌표계 / 이동 행렬
	
	glm::vec3 v(5, 6, 1);
	glm::mat3 t(1.0);
	t = glm::translate(t, glm::vec2(2, 3));

	v = t * v;

	std::cout << glm::to_string(t) << std::endl;
	std::cout << glm::to_string(v) << std::endl;

	*/

	glm::vec3 v(5, 6, 1);
	glm::mat3 t(1.0);
	t = glm::scale(t, glm::vec2(3, 4));

	v = t * v;

	std::cout << glm::to_string(t) << std::endl;
	std::cout << glm::to_string(v) << std::endl;

	return 0;
}