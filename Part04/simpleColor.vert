#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec4 colorValue;
void main(){

    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);


}

