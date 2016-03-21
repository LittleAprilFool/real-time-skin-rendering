#version 400

in  vec4 vPosition;
in vec2 vTexcoord;
in vec4 vNormal;
out float gn;
out vec2 texcoord;
out vec3 lcolor;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 model_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 Kd;
uniform vec3 global_ambient;

void main()
{
  //compute position
  vec4 position;
  position = vPosition;
  position = position + vec4(model_pos, 0.0);
  gl_Position = projection * model_view * position;
  
  gn = 0;
  texcoord = vTexcoord;

  //compute L
  vec3 L = vPosition.xyz - light_pos;
  L = normalize(L);
  vec3 N = vNormal.xyz;
  N = normalize(N);
  
  //compute diffuseColor
  vec3 diffuseColor = Kd * light_color * max(dot(N, L), 0);
  
  //compute ambientColor 
  vec3 ambientColor = Kd * global_ambient;


  lcolor = diffuseColor + ambientColor;

  
}