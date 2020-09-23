#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform float time;



const float blurSizeH = 1.0 / 1024;
const float blurSizeV = 1.0 / 768;
const vec2 targetSize =vec2(1024,768);
vec4 boxFilter(vec2 UV)
{
	
	//box blur
	vec4 sum = vec4(0.0);
	for (int x = -4; x <= 4; x++)
	for (int y = -4; y <= 4; y++)
	sum += texture(
	renderedTexture,
	vec2(UV.x + x * blurSizeH, UV.y + y * blurSizeV)
	) / 81.0;
 
	return sum;
}


vec4 sobel(vec2 UV)
{
	
	
	//sobel
	vec4 top         = texture(renderedTexture, vec2(UV.x, UV.y + 1.0 / targetSize.y));
	vec4 bottom      = texture(renderedTexture, vec2(UV.x, UV.y - 1.0 / targetSize.y));
	vec4 left        = texture(renderedTexture, vec2(UV.x - 1.0 / targetSize.x, UV.y));
	vec4 right       = texture(renderedTexture, vec2(UV.x + 1.0 / targetSize.x, UV.y));
	vec4 topLeft     = texture(renderedTexture, vec2(UV.x - 1.0 / targetSize.x, UV.y + 1.0 / targetSize.y));
	vec4 topRight    = texture(renderedTexture, vec2(UV.x + 1.0 / targetSize.x, UV.y + 1.0 / targetSize.y));
	vec4 bottomLeft  = texture(renderedTexture, vec2(UV.x - 1.0 / targetSize.x, UV.y - 1.0 / targetSize.y));
	vec4 bottomRight = texture(renderedTexture, vec2(UV.x + 1.0 / targetSize.x, UV.y - 1.0 / targetSize.y));
	vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
	vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
	vec4 sobel = sqrt(sx * sx + sy * sy);
	return sobel;
}



void main(){
	//color = texture( renderedTexture, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz ;
	//color = boxFilter(UV).xyz;
	color  = sobel (UV).xyz;
}

