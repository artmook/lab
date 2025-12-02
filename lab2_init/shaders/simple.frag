#version 450

in vec3 fColor;

//프레그먼트 쉐이더는 반드시 하나 이상의 out vec4가 있어야 함
out vec4 fregColor; //픽셀의 색상을 의미, 왜 vec4? R, G, B, A
void main()
{
	fregColor = vec4(fColor, 1.0);
}