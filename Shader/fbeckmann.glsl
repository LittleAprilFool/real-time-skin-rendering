#version 400

in vec2 texcoord;
out vec4 fColor;


float PHBeckmann(float ndoth, float m)
{
	float alpha = acos(ndoth);
	float ta = tan(alpha);
	float val = 1.0 / (m * m * pow(ndoth, 4.0)) * exp(-(ta * ta) / (m * m));
	return val; 
}

float BeckmannTextureCompute(vec2 tex)
{ 
	return 0.5 * pow(PHBeckmann(tex.x, tex.y), 0.1); 
}

void main()
{ 
	float color = BeckmannTextureCompute(texcoord);
	fColor = vec4(color, color, color, 1);
	//fColor = vec4(1,1,1,1);
}