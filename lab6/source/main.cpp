#define GLM_ENABLE_EXPERIMENTAL
#include<iostream>
#include <glm/gtx/dual_quaternion.hpp>
#include<glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include<glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
#include<glm/mat4x4.hpp>
#include<glm/mat2x2.hpp>
#include<glm/mat3x2.hpp>
#include<glm/mat3x3.hpp>

//glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up);
//
//int main() {
//    glm::vec4 local_point(4, 5, 3, 1);
//
//	glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.3f, 1.3f));
//	glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f));
//
//    glm::mat4 x = glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    glm::mat4 y = glm::rotate(glm::mat4(1.0f), glm::radians(56.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 z = glm::mat4(1.0f);
//
//    glm::mat4 r = x * y * z;
//
//	glm::mat4 model = t * r * s;
//    glm::vec4 world_point = model * local_point;
//
//    //std::cout << to_string(world_point) << std::endl;
//
//    glm::mat4 view = { {0.83f, -0.3f, 0.47f, 0.0f}, 
//                       {0.0f, 0.84f, 0.54f, 0.0f},
//                       {-0.56f, -0.45f, 0.70f, 0.0f},
//                       {0.0f, 0.0f, -44.47f, 1.0f} };
//
//	//view 행렬은 world로 부터 camera 좌표계로 변환하는 행렬
//	//inverse(view) 행렬은 camera 좌표계로 부터 world 좌표계로 변환하는 행렬
//
//	glm::mat4 inView = glm::inverse(view); //camera -> world
//    //inverce의 마지막 칼럼이 카메라의 위치
//
//
//    glm::vec3 camPos = inView[3]; // 카메라 위치
//    //view 구성 시, 1개의 축을 구했는데 카메라는 -z축을 바라본다고 가정
//
//    //카메라는 -z축을 바라보고 있으므로
//	glm::vec3 z_axis = glm::normalize(-inView[2]);
//	glm::vec3 look = camPos + z_axis * 10.0f;
//	glm::vec3 up(0, 1, 0);
//    glm::mat4 myView = lookAt(camPos, look, up);
//
//	std::cout << "View\n" << to_string(view) << std::endl;
//	std::cout << "myView\n" << to_string(myView) << std::endl;
//
//    return 0;
//}
//
///*
//int main() {
//
//    std::cout << to_string(lookat({ 5, 5, 5 }, { 0, 0, 0 }, { 0, 1, 0 })) << std::endl;
//
//    return 0;
//} */
//
//glm::mat4 lookAt(glm::vec3 camPos, glm::vec3 Look, glm::vec3 Up) {
//    glm::vec3 z = glm::normalize(camPos - Look);      // 새 Z축
//    glm::vec3 x = glm::normalize(glm::cross(Up, z));  // 새 X축
//    glm::vec3 y = glm::cross(z, x);                   // 새 Y축
//    glm::mat4 t = glm::translate(glm::mat4(1.0f), -camPos);
//    glm::mat4 r({ x.x, y.x, z.x, 0 }, { x.y, y.y, z.y, 0 }, { x.z, y.z, z.z, 0 }, { 0,   0,   0,   1 });
//
//    return r * t;
//}

/*#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_transform_2d.hpp> 
#include <glm/mat3x2.hpp> */       
glm::mat4 perspective(float fovy, float aspect, float near, float far)
{
    glm::mat4 projection(0.0f);
    float a = 1.0f / (aspect * tan(glm::radians(fovy) / 2.0f));
    float b = 1.0f / tan(glm::radians(fovy) / 2.0f);
    float c = -((far + near) / (far - near));
    float d = -(2.0f * far * near / (far - near));

    projection[0][0] = a;
    projection[1][1] = b;
    projection[2][2] = c;
    projection[2][3] = -1.0f;
    projection[3][2] = d;

    return projection;
}

int main()

{

    float width = 780.0f;
    float height = 750.0f;
    float fovY = 45.0f;
    float nearz = 0.1f;
    float farz = 500.0f;
    float aspect = width / height;

    glm::mat4 perspectiveMat = perspective(fovY, aspect, nearz, farz);
    std::cout << glm::to_string(perspectiveMat) << std::endl;

    return 0;
}