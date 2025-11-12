
#define _USE_MATH_DEFINES 
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>   //STL(Standard Template Library) vector

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <chrono>       //시간 체크

#include "cow.h"
#include "ppm_io.h"


void convertNDCtoImage(const glm::vec4 vertexNDC, glm::vec4 vertexView, const uint32_t& imageWidth, const uint32_t& imageHeight, glm::vec3& vertexRaster)
{
    vertexRaster.x = ((vertexNDC.x + 1.0f) / 2) * imageWidth;
    vertexRaster.y = ((1.0f - vertexNDC.y) / 2) * imageHeight;
    vertexRaster.z = -vertexView.z;
}



float min3(const float& a, const float& b, const float& c)
{
    return std::min(a, std::min(b, c));
}

float max3(const float& a, const float& b, const float& c)
{
    return std::max(a, std::max(b, c));
}


//벡터에 포인터가 오른쪽인지 확인 하는 함수
bool edge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)  // a->b로 가는 벡터에 대해 c가 오른쪽인지 왼쪽인지 확인
{
    return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
}

//세 개의 꼭짓점으로 이루어진 삼각형의 면적 구하기
float area(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
    return fabs((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}


glm::mat4 lookAt(glm::vec3 camPos, glm::vec3 Look, glm::vec3 Up)
{
    glm::vec3 z = glm::normalize(camPos - Look);
    glm::vec3 x = glm::normalize(glm::cross(Up, z));
    glm::vec3 y = glm::cross(z, x);
    glm::mat4 t = glm::translate(glm::mat4(1.0f), -camPos);
    glm::mat4 r({ x.x, y.x, z.x, 0 }, { x.y, y.y, z.y, 0 }, { x.z, y.z, z.z, 0 }, { 0,   0,   0,   1 });

    return r * t;
}

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

const uint32_t imageWidth = 640;
const uint32_t imageHeight = 480;


const uint32_t ntris = 3156;
const float nearClippingPlane = 1;
const float farClippingPLane = 1000;



int main(int argc, char** argv)
{
  
    glm::mat4 worldToCamera = glm::lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));

    float t, b, l, r;

    PPM ppmOut;
    ppmOut.setBinary(true);



    struct rgb
    {
        uint8_t r, g, b;
    };
    //640, 480 크기만큼의 rgb를 가지고 있음
	std::vector <rgb>  frameBuffer; // rgb라는 구조체를 요소로 가지는 벡터 frameBuffer 선언
    frameBuffer.resize(imageWidth * imageHeight);


    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        frameBuffer[i].r = 0;
        frameBuffer[i].g = 0;
        frameBuffer[i].b = 0;
    }

    float* depthBuffer = new float[imageWidth * imageHeight];

    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
		depthBuffer[i] = farClippingPLane;  //초기값은 가장 먼 값으로 설정
    }

    auto t_start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < ntris; ++i) {

        //로컬(모델) 좌표
        const glm::vec3& v0 = vertices[nvertices[i * 3]];       //삼각형의 첫번째 꼭짓점
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];   //삼각형의 두번째 꼭짓점
        const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];   //삼각형의 세번째 꼭짓점
        
		//std::cout << "local" << std::endl << glm::to_string(v0) << std::endl << glm::to_string(v1) << std::endl << glm::to_string(v2) << std::endl << std::endl;

        //모델 행렬 : 로컬 -> 글로벌
        glm::mat4 modelMatrix(1.0f);
        //뷰 행렬 : 글로벌 -> 카메라
        glm::mat4 viewMatrix = lookAt(glm::vec3(0, 20, 40), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
		//모델 뷰 행렬 : 로컬 -> 카메라
        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        //Camera(view) coordinates
        glm::vec4 v0e; // e : eye(camera)
        glm::vec4 v1e;
        glm::vec4 v2e;

        v0e = modelViewMatrix * glm::vec4(v0, 1.0); //x, y, z, 1 / vsc4 안해주면 vec4 * vec3 이라 오류생김, 마지막에 1을 넣은 이유 -> 동차 좌표떄문에 넣음
        v1e = modelViewMatrix * glm::vec4(v1, 1.0);
        v2e = modelViewMatrix * glm::vec4(v2, 1.0);

		//std::cout << "view" << std::endl << glm::to_string(v0e) << std::endl << glm::to_string(v1e) << std::endl << glm::to_string(v2e) << std::endl << std::endl;

        //모델 뷰 행렬 * 로컬 좌표계
        //Code here (calculate v0e, v1e, v2e here)


        glm::mat4 projection = perspective((45.0f), imageWidth / (float)imageHeight, nearClippingPlane, farClippingPLane);

        //클립 좌표 = 프로젝션 행렬 * 뷰 좌표

        //Clip coodinates
        glm::vec4 v0c, v1c, v2c;  // 동차 좌표 때문에 4차원 / w = 1
        
        v0c = projection * v0e;
        v1c = projection * v1e;
        v2c = projection * v2e;

        //std::cout << "clip" << std::endl << glm::to_string(v0c) << std::endl << glm::to_string(v1c) << std::endl << glm::to_string(v2c) << std::endl << std::endl;

        //Code here (calculate v0c, v1c, v2c here)
		//Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w   (same to v1c, v2c)

        //x, y, z 를 w로 나누기
        //모든 좌표값을 [-1, 1]로 바꾸기 위해 : NDC
		v0c.x /= v0c.w; v0c.y /= v0c.w; v0c.z /= v0c.w;
		v1c.x /= v1c.w; v1c.y /= v1c.w; v1c.z /= v1c.w;
		v2c.x /= v2c.w; v2c.y /= v2c.w; v2c.z /= v2c.w;

        //NDC : v0c, v1c, v2c




        //Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w   (same to v1c, v2c)

        //Code here

        //std::cout << "NDC" << std::endl << glm::to_string(v0c) << std::endl << glm::to_string(v1c) << std::endl << glm::to_string(v2c) << std::endl << std::endl;

        glm::vec3 v0Raster, v1Raster, v2Raster;
        convertNDCtoImage(v0c, v0e, imageWidth, imageHeight, v0Raster);
        convertNDCtoImage(v1c, v1e, imageWidth, imageHeight, v1Raster);
        convertNDCtoImage(v2c, v2e, imageWidth, imageHeight, v2Raster);

        //std::cout << "Raster" << std::endl << glm::to_string(v0Raster) << std::endl << glm::to_string(v1Raster) << std::endl << glm::to_string(v2Raster) << std::endl << std::endl;


      

        float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y);
        float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x);
        float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y);

        if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

        uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
        uint32_t x1 = std::min(int32_t(imageWidth) - 1, (int32_t)(std::floor(xmax)));
        uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
        uint32_t y1 = std::min(int32_t(imageHeight) - 1, (int32_t)(std::floor(ymax)));


        //calculat the area of triangle  (area)
        float total = area(v0Raster, v1Raster, v2Raster);

        v0Raster.z = 1.0f / v0Raster.z,
        v1Raster.z = 1.0f / v1Raster.z,
        v2Raster.z = 1.0f / v2Raster.z;

		//삼각형의 min, max 값을 구해서 그 범위 내에서 픽셀을 돌면서 체크(바운딩 박스)
		//이유 : 픽셀을 하나하나 반복문으로 돌리면 오래 걸림, 삼각형의 외곽에 있는 픽셀들을 굳이 다 체크할 필요가 없기 때문
        for (uint32_t y = y0; y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {

                glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);

                //calculate the areas of  three suvdivided triangles

                bool w0 = edge(v1Raster, v2Raster, pixelSample); //w0
                bool w1 = edge(v2Raster, v0Raster, pixelSample); //w1
                bool w2 = edge(v0Raster, v1Raster, pixelSample); //w2


                if (w0 == true && w1 == true && w2 == true) { //inside

                    float a0 = area(v0Raster, v1Raster, pixelSample);
                    float a1 = area(v1Raster, v2Raster, pixelSample);
                    float a2 = area(v2Raster, v0Raster, pixelSample);

                    //calculate the ratio here
                    a0 /= total;
                    a1 /= total;
                    a2 /= total;

                    //calculate the z of pixelSample
					float z = 0.0f; //pixelSample의 z값을 파악해야함 : pixelSample의 카메라까지의 거리

                    

                    //code here


                    //depth 버퍼에는 해당 픽셀의 가장 가까운 거리값(카메라까지의)이 들어있음
                    if (z < depthBuffer[y * imageWidth + x]) {

                        depthBuffer[y * imageWidth + x] = z;


                        //calculate normal vector from v0e, v1e, v2e
                        //normal > 1

						glm::vec3 e0 = glm::vec3(v1e) - glm::vec3(v0e);
						glm::vec3 e1 = glm::vec3(v2e) - glm::vec3(v0e);

						glm::vec3 n = glm::normalize(glm::cross(e0, e1));
                        
						glm::vec3 c = (n + 1.0f) * 0.5f; // n 을 [-1, 1] -> [0, 1]로 변환

                        //glm::vec3 n;

						//n -> x,y,z가 모두 [-1, 1] -> [0, 1]

                        frameBuffer[y * imageWidth + x].r = c.r * 255.0f;
                        frameBuffer[y * imageWidth + x].g = c.g * 255.0f;
                        frameBuffer[y * imageWidth + x].b = c.x * 255.0f;

                    }
                }
                
  
            }  // 안쪽 for loop
 
        }// 바깥쪽 for loop

    }

    auto t_end = std::chrono::high_resolution_clock::now();
    auto passedTime = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    std::cerr << "Wall passed time: " << passedTime << "ms" << std::endl;


    rgb* p = frameBuffer.data();

    ppmOut.load(&p[0].r, imageHeight, imageWidth, 255, "P6");
    ppmOut.write("../../output.ppm");

    delete[] depthBuffer;

    return 0;

}