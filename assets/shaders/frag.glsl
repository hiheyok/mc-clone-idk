#version 450 core

out vec4 final;

in float lights;
in float texturePosition;
in vec3 FragPos;  
in vec2 textureSize;

uniform vec3 camPos;
uniform float RenderDistance;
uniform sampler2DArray BlockTexture;

float near = 0.001; 
float far  = RenderDistance; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() 
{

	// float depth = LinearizeDepth(gl_FragCoord.z) / far;

	float tsx = textureSize.x;
	float tsy = textureSize.y;
		

	vec4 texture_ =  vec4(texture(BlockTexture, vec3(vec2(tsx,tsy).xy, texturePosition - 1)).rgb , 1);
	
	
//	float depth = 1 ;//- (LinearizeDepth(gl_FragCoord.z) / far);
	final = texture_ * vec4(lights,lights, lights,1.0);
	
	
}