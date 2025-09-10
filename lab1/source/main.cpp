#define GLM_ENABLE_EXPERIMENTAL 
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
//using namespace glm; ��� �� ����

/*

//glm �� ��ü ������ �ƴ϶� c�� ����ϴ�
int main()
{
	glm::vec2 a(1.0f, 4.0f);
	//2���� ������ ����?
	// a.length(); -> ������ ��ȯ���ִ� �� (2����, 3����...)
	//������ ���̸� ���ϴ� ���� / ���̴� ��Į�󿩼� ������ float�� �޾������
	float l = glm::length(a);

	glm::vec3 b(1.0f, 4.0f, 2.0f);

	//b = b * 3.0 <- ����, b�� float�� 3.0�� double�̶�

	glm::vec3 c = glm::vec3(1, 1, 1);

	//glm::normalize(a); << �̰� normalize�� ����ϸ� �ȵ�, �޴°� ���� -> �߰���� �� Ʋ���� ����
	//d = glm::normalize(c); ��ȯ���� �޾������
	glm::vec3 d = glm::normalize(c);

	c.xyz = d.xyz; //c.x = d.x; c.y = d.y; c.z = d.z;

	std::cout << glm::to_string(a) << std::endl;
	

	//glm::normalize(a); << �̰� normalize�� ����ϸ� �ȵ�, �޴°� ���� -> �߰���� �� Ʋ���� ����
	c = glm::normalize(c); //��ȯ���� �޾������



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