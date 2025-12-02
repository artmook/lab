#define _USE_MATH_DEFINES
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector> //STL(Standard Template Library) vector
#include <algorithm> // std::max

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <chrono> //시간 체크

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
bool edge(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) // a->b로 가는 벡터에 대해 c가 오른쪽인지 왼쪽인지 확인
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
    glm::mat4 r({ x.x, y.x, z.x, 0 }, { x.y, y.y, z.y, 0 }, { x.z, y.z, z.z, 0 }, { 0, 0, 0, 1 });

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
    std::vector <rgb> frameBuffer; // rgb라는 구조체를 요소로 가지는 벡터 frameBuffer 선언
    frameBuffer.resize(imageWidth * imageHeight);

    // 프레임버퍼와 뎁스버퍼를 먼저 초기화합니다.
    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        frameBuffer[i].r = 0;
        frameBuffer[i].g = 0;
        frameBuffer[i].b = 0;
    }

    float* depthBuffer = new float[imageWidth * imageHeight];
    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) {
        depthBuffer[i] = farClippingPLane; //초기값은 가장 먼 값으로 설정
    }

    // -------------------------------------------------------------------
    // [퐁 셰이딩] 1. 'my_normals' 배열 계산 (전처리)
    // -------------------------------------------------------------------

    // `vertices` 배열의 실제 크기(`vertices[1732]`)가 1732개 이므로, 그 값을 사용합니다.
    const uint32_t nverts_total = 1732;

    // [퐁 셰이딩] 노멀을 저장할 임시 벡터(배열) 선언 (모두 0으로 초기화)
    std::vector<glm::vec3> my_normals(nverts_total, glm::vec3(0.0f));

    // [퐁 셰이딩] 1-A. 면 노멀 "누적" (Accumulation)
    // 모든 삼각형을 순회하며 면 노멀을 계산하고, 각 꼭지점에 더해줍니다.
    for (uint32_t i = 0; i < ntris; ++i) {
        uint32_t idx0 = nvertices[i * 3];
        uint32_t idx1 = nvertices[i * 3 + 1];
        uint32_t idx2 = nvertices[i * 3 + 2];

        const glm::vec3& v0 = vertices[idx0];
        const glm::vec3& v1 = vertices[idx1];
        const glm::vec3& v2 = vertices[idx2];

        // [퐁 셰이딩] 이 삼각형의 면 노멀을 계산
        glm::vec3 face_normal = glm::cross(v1 - v0, v2 - v0);

        // [퐁 셰이딩] 면 노멀을 3개의 꼭지점에 각각 더함
        my_normals[idx0] += face_normal;
        my_normals[idx1] += face_normal;
        my_normals[idx2] += face_normal;
    }

    // [퐁 셰이딩] 1-B. 노멀 "정규화" (Normalization)
    // 누적된 노멀들을 정규화(길이 1로 만듦)하여 최종 꼭지점 노멀을 완성합니다.
    for (uint32_t i = 0; i < nverts_total; ++i) {
        // [퐁 셰이딩] 만약 더해진 노멀이 0벡터이면 (사용되지 않는 꼭지점 등) 
        // normalize 시 NaN가 될 수 있으므로, 0벡터가 아닐 때만 수행합니다.
        if (glm::length(my_normals[i]) > 0.0001f) {
            my_normals[i] = glm::normalize(my_normals[i]);
        }
    }
    // [퐁 셰이딩] 전처리 완료. `my_normals` 배열이 준비되었습니다.
    // -------------------------------------------------------------------


    auto t_start = std::chrono::high_resolution_clock::now();

    // [퐁 셰이딩] 2. 렌더링 루프
    for (uint32_t i = 0; i < ntris; ++i) {

        // [퐁 셰이딩] 2-A. 로컬(모델) 공간의 노멀을 가져옴
        // 위에서 계산한 `my_normals` 배열을 사용합니다.
        const glm::vec3& v0n_local = my_normals[nvertices[i * 3]];
        const glm::vec3& v1n_local = my_normals[nvertices[i * 3 + 1]];
        const glm::vec3& v2n_local = my_normals[nvertices[i * 3 + 2]];

        //로컬(모델) 좌표
        const glm::vec3& v0 = vertices[nvertices[i * 3]]; //삼각형의 첫번째 꼭짓점
        const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]]; //삼각형의 두번째 꼭짓점
        const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]]; //삼각형의 세번째 꼭짓점

        //std::cout << "local" << std::endl << glm::to_string(v0) << std::endl << glm::to_string(v1) << std::endl << glm::to_string(v2) << std::endl << std::endl;

        //모델 행렬 : 로컬 -> 글로벌
        glm::mat4 modelMatrix(1.0f);
        //뷰 행렬 : 글로벌 -> 카메라
        glm::mat4 viewMatrix = lookAt(glm::vec3(20, 10, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
        //모델 뷰 행렬 : 로컬 -> 카메라
        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        // [퐁 셰이딩] 2-B. 노멀을 뷰 공간으로 변환
        // 노멀은 방향이므로, modelViewMatrix의 '역전치(Inverse Transpose)' 행렬을 곱합니다.
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
        glm::vec3 v0n_view = glm::normalize(normalMatrix * v0n_local);
        glm::vec3 v1n_view = glm::normalize(normalMatrix * v1n_local);
        glm::vec3 v2n_view = glm::normalize(normalMatrix * v2n_local);

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
        glm::vec4 v0c, v1c, v2c; // 동차 좌표 때문에 4차원 / w = 1
        v0c = projection * v0e;
        v1c = projection * v1e;
        v2c = projection * v2e;

        //std::cout << "clip" << std::endl << glm::to_string(v0c) << std::endl << glm::to_string(v1c) << std::endl << glm::to_string(v2c) << std::endl << std::endl;

        //Code here (calculate v0c, v1c, v2c here)
        //Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w (same to v1c, v2c)

        //x, y, z 를 w로 나누기
        //모든 좌표값을 [-1, 1]로 바꾸기 위해 : NDC
        v0c.x /= v0c.w; v0c.y /= v0c.w; v0c.z /= v0c.w;
        v1c.x /= v1c.w; v1c.y /= v1c.w; v1c.z /= v1c.w;
        v2c.x /= v2c.w; v2c.y /= v2c.w; v2c.z /= v2c.w;

        //NDC : v0c, v1c, v2c

        //Perspective division (divide v0c.x, v0c.y, v0c.z by v0c.w (same to v1c, v2c)

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

        //calculat the area of triangle (area)
        float total = area(v0Raster, v1Raster, v2Raster);
        if (total < 1e-5) continue; // [퐁 셰이딩] 면적이 거의 0인 삼각형은 스킵 (0으로 나누기 방지)

        // [퐁 셰이딩] 2-C. "원근 보정 보간" 준비
        // 픽셀에서 1/z 와 N/z (노멀/z) 를 보간하기 위해 값을 미리 계산합니다.
        float z0_view = v0Raster.z; // 뷰 공간 z값을 임시 저장
        float z1_view = v1Raster.z;
        float z2_view = v2Raster.z;

        v0Raster.z = 1.0f / z0_view; // 래스터 z값에 1/z를 저장
        v1Raster.z = 1.0f / z1_view;
        v2Raster.z = 1.0f / z2_view;

        glm::vec3 v0n_interp = v0n_view / z0_view; // N/z 값을 계산
        glm::vec3 v1n_interp = v1n_view / z1_view;
        glm::vec3 v2n_interp = v2n_view / z2_view;

        //삼각형의 min, max 값을 구해서 그 범위 내에서 픽셀을 돌면서 체크(바운딩 박스)
        //이유 : 픽셀을 하나하나 반복문으로 돌리면 오래 걸림, 삼각형의 외곽에 있는 픽셀들을 굳이 다 체크할 필요가 없기 때문
        for (uint32_t y = y0; y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {

                glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);

                //calculate the areas of three suvdivided triangles

                bool w0 = edge(v1Raster, v2Raster, pixelSample); //w0
                bool w1 = edge(v2Raster, v0Raster, pixelSample); //w1
                bool w2 = edge(v0Raster, v1Raster, pixelSample); //w2

                if (w0 == true && w1 == true && w2 == true) { //inside

                    // [퐁 셰이딩] 2-D. Barycentric 좌표 (가중치) 계산
                    // area 함수가 2D 좌표에서 작동하므로 v*Raster의 z값을 사용하면 안됩니다.
                    // (다행히 area 함수는 z값을 무시하므로 v*Raster를 그대로 써도 괜찮습니다.)
                    float a0 = area(v1Raster, v2Raster, pixelSample) / total; // v0에 대한 가중치
                    float a1 = area(v2Raster, v0Raster, pixelSample) / total; // v1에 대한 가중치
                    float a2 = area(v0Raster, v1Raster, pixelSample) / total; // v2에 대한 가중치

                    // [퐁 셰이딩] 2-E. 픽셀의 z값 보간 (원근 보정)
                    float one_over_z = a0 * v0Raster.z + a1 * v1Raster.z + a2 * v2Raster.z; // 1/z를 보간
                    float z = 1.0f / one_over_z; // 다시 역수를 취해 z값을 복원

                    // [퐁 셰이딩] (수정) 'float z = 0.0f;' 대신 보간된 z값으로 깊이 테스트
                    //if (z < depthBuffer[y * imageWidth + x]) { // 기존 코드
                    if (z > nearClippingPlane && z < depthBuffer[y * imageWidth + x]) { // [퐁 셰이딩] near 클립 바깥, 뎁스 버퍼보다 가까울 때
                        depthBuffer[y * imageWidth + x] = z;

                        // [퐁 셰이딩] 2-F. 픽셀의 노멀 보간 (슬라이드 핵심)
                        // N/z 를 보간합니다.
                        glm::vec3 n_interp = a0 * v0n_interp + a1 * v1n_interp + a2 * v2n_interp;
                        // z를 다시 곱하고 정규화하여 픽셀의 최종 노멀(n)을 복원합니다.
                        glm::vec3 n = glm::normalize(n_interp * z);

                        // [퐁 셰이딩] (삭제) 기존 플랫 셰이딩 노멀 계산 코드를 삭제합니다.
                        // glm::vec3 e0 = glm::vec3(v1e) - glm::vec3(v0e);
                        // glm::vec3 e1 = glm::vec3(v2e) - glm::vec3(v0e);
                        // glm::vec3 n = glm::normalize(glm::cross(e0, e1));

                        // [퐁 셰이딩] 보간된 노멀 n 을 사용해 색상 계산
                        glm::vec3 c = (n + 1.0f) * 0.5f; // n 을 [-1, 1] -> [0, 1]로 변환

                        //glm::vec3 n; // 기존 코드

                        //n -> x,y,z가 모두 [-1, 1] -> [0, 1] // 기존 주석

                        frameBuffer[y * imageWidth + x].r = c.r * 255.0f;
                        frameBuffer[y * imageWidth + x].g = c.g * 255.0f;
                        frameBuffer[y * imageWidth + x].b = c.z * 255.0f; // [퐁 셰이딩] (오류 수정) c.x -> c.z
                    }
                }
            } // 안쪽 for loop
        } // 바깥쪽 for loop
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