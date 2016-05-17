#version 400

layout(location = 0) out vec4 fDepth;

void main()
{
	//fDepth = gl_FragCoord.z;
	//int value = gl_FragCoord.z;
	fDepth = vec4(gl_FragCoord.z,0,0,1);
	//fDepth = vec4(0,0,0,1);
}