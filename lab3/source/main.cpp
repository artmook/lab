#define GLM_ENABLE_EXPERIMENTAL 
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>

const double DEGREES_TO_RADIANS = glm::pi<float>() / 180.0;
const double RADIANS_TO_DEGREES = 180.0 / glm::pi<float>();

int main() {
	/*
	* 
	* ���� ���
	* 
	* �����̶�? -> �� ���Ϳ� ��� ������ ���ο� ���͸� ���ϴ� ����
	* 

	glm::vec3 u(1, 0, 0);
	glm::vec3 v(-1, 1, 0);

	glm::vec3 c = glm::cross(u, v);	// ���� u�� v�� ���� ���

	float l = glm::length(c);	//����纯�� ���� ���ϱ�
	//������ �ؼ� u�� v�� ���̰��� ����
	float angle = glm::acos(glm::dot(u, v) / (glm::length(u) * glm::length(v)));

	float l2 = glm::length(u) * glm::length(v) * glm::sin(angle); //����纯�� ���� ���ϱ�

	glm::vec3 u2(1, 0, 0);	//�غ�
	glm::vec3 v2(0, 1, 0);	//����
	float area = glm::length(glm::cross(u2, v2));

	glm::vec3 u3(1, 0, 0);
	glm::vec3 v3(1, 0, 0);

	float area2 = glm::length(glm::cross(u3, v3));

	*/

	glm::vec3 p1(3.0f, 0.0f, 0.0f);
	glm::vec3 p2(1.5f, 0.86f, 0.0f);
	glm::vec3 p3(3.0f, 0.0f, -1.0f);

	glm::vec3 v1 = p3 - p2;
	glm::vec3 v2 = p1 - p2;
	glm::vec3 normal = glm::cross(v1, v2);

	normal = glm::normalize(normal);
	std::cout << glm::to_string(normal) << std::endl;
	

	return 0;
}