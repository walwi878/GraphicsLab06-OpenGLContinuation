#version 330 core

// Input vertex data, different for all executions of this shader.
// location is defined by attribute index (set by glVertexAttribPointer in Mesh.cpp)
layout(location = 0) in vec3 vertexPosModelspace;	// 0 is index for vertices
layout(location = 1) in vec2 vertexUV;				// 1 is index for texture coordinates
layout(location = 2) in vec3 vertexNormalModelspace;// 2 is index for normals

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 posWorldspace;
out vec3 normalCameraspace;
out vec3 eyeDirectionCameraspace;
out vec3 lightDirectionCameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPosWorldspace;

void main(){
    
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosModelspace,1);
    
    // Position of the vertex, in worldspace : M * position
    posWorldspace = (M * vec4(vertexPosModelspace,1)).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosCameraspace = ( V * M * vec4(vertexPosModelspace,1)).xyz;
    eyeDirectionCameraspace = vec3(0,0,0) - vertexPosCameraspace;
    
    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    vec3 lightPositionCameraspace = ( V * vec4(lightPosWorldspace,1)).xyz;
    lightDirectionCameraspace = lightPositionCameraspace + eyeDirectionCameraspace;
    
    // Normal of the the vertex, in camera space
    normalCameraspace = ( V * M * vec4(vertexNormalModelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    
    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}

