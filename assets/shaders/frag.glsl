#version 450 core

out vec4 final;

in float lights;
in float texturePosition;
in vec3 FragPos;  
in vec2 textureSize;

uniform vec3 camPos;
uniform float RenderDistance;
uniform sampler2DArray BlockTexture;

float near = 0.25; 
float far  = RenderDistance; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
//vec3 skyColor = vec3(0.46274509803f, 0.65882352941f,1.0f,1.0f);


void main() 
{

	float depth = 1 - LinearizeDepth(gl_FragCoord.z) / far;

	depth = pow(depth , 4);

	float tsx = textureSize.x;
	float tsy = textureSize.y;

	vec4 texture_ =  vec4(texture(BlockTexture, vec3(vec2(tsx,tsy).xy, texturePosition - 1)).rgb , 1);

	vec4 color = (texture_ * vec4(lights,lights, lights,1.0f));

	float depthr = color.r - ((1 - depth) * (color.r - 0.46274509803f));
	float depthg = color.g - ((1 - depth) * (color.g - 0.65882352941f));
	float depthb = color.b - ((1 - depth) * (color.b - 1.0f));
	

	final = vec4(depthr, depthg, depthb, 1.0f);
}