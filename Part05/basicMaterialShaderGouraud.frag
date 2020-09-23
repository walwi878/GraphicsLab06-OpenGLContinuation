#version 330 core
// Gouraud
// Interpolated values from the vertex shaders

in vec4 color;
// Ouput data
out vec3 outcolor;

void main(){
	
	outcolor = color.rgb;

}


