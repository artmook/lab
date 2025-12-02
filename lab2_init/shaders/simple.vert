#version 450 

//cpu로 부터 받은 속성 0번의 입력을 vec4로 반응
layout(location = 0) in vec4 vertexPositoin;
//cpu로 부터 받은 속성 1번의 입력을 vec4로 반응
layout(location = 1) in vec3 vertexColor;

uniform mat4 mvp;


out vec3 fColor; //fragment shader로 보낼 데이터
void main()
{
	//gl_Position은 빌트인 키워드로써, 클립좌표게를 받아 들인다
	//나중에 자동으로 gl_Position 값을 perspective division을 해줌

	//그런데 왜 좌표계 변환을 안하지? 이유는 지금 받은 vertexPosition이 클립 좌표계로 가정했기 때문
	//그렇기 때문에 곧바로 넣을 수 있음
	//증료! : vertex shader에는 반드시 gl_Position값을 설정해야 함
	gl_Position = mvp * vertexPositoin;
	fColor = vertexColor;

}
