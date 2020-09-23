#version 330 core

// Input vertex data, different for all executions of this shader.
// location is defined by attribute index (set by glVertexAttribPointer in Mesh.cpp)
layout(location = 0) in vec3 vertexPosModelspace;	// 0 is index for vertices
layout(location = 1) in vec2 vertexUV;				// 1 is index for texture coordinates
layout(location = 2) in vec3 vertexNormalModelspace;// 2 is index for normals



// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPosWorldspace;
uniform sampler2D myTextureSampler;


const float ns = 6.0; //specular exponent

// Light emission properties
const vec3 ambientLightColor = vec3(0.4,0.4,0.4);
const vec3 diffuseLightColor = vec3(1.0,1.0,1.0);
const vec3 specularLightColor = vec3(1.0,1.0,1.0);

out vec4 color; //color

void main(){
	
	//would come from a material settings file (e.g. mtl)
	vec3 ambientMatColor = vec3(0.6,0.6,0.6);
	vec3 diffuseMatColor = vec3(1.0,1.0,1.0);
	vec3 specularMatColor = vec3(0.3,0.3,0.3);
	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosModelspace,1);
	
	// Position of the vertex, in worldspace : M * position
	vec3 posWorldspace = (M * vec4(vertexPosModelspace,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosCameraspace = ( V * M * vec4(vertexPosModelspace,1)).xyz;
	vec3 eyeDirectionCameraspace = vec3(0,0,0) - vertexPosCameraspace;
	
	// Material properties
	vec3 textureVal = texture( myTextureSampler, vertexUV ).rgb;  //texture map will be used for diffuse
	
	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPositionCameraspace = ( V * vec4(lightPosWorldspace,1)).xyz;
	vec3 lightDirectionCameraspace = lightPositionCameraspace + eyeDirectionCameraspace;
	
	// Normal of the the vertex, in camera space
	vec3 normalCameraspace = ( V * M * vec4(vertexNormalModelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	
	// We now work in camera space
	// Normal of the computed fragment, in camera space
	vec3 N = normalize( normalCameraspace );
	// Direction of the light (from the fragment to the light) in camera space
	vec3 L = normalize( lightDirectionCameraspace );
	
	// We use the cosine of the angle theta between the normal and the light direction to compute the diffuse component.
	// The cosine is clamped to contrain it between 0 and 1
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( N,L), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(eyeDirectionCameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-L,N);
	
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// The cosine is clamped to contrain it between 0 and 1 to avoid negative values
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	vec3 diffuseComponent = diffuseLightColor* diffuseMatColor * textureVal * cosTheta;
	vec3 ambientComponent = ambientLightColor * ambientMatColor * textureVal; //for simplification we reuse the diffuse texture map for the ambient texture map
	vec3 specularComponent = specularLightColor * specularMatColor  * pow(cosAlpha,ns);
	
	color.rgb =
	// Ambient : simulates indirect lighting
	ambientComponent +
	// Diffuse : "color" of the object
	diffuseComponent +
	// Specular : reflective highlight, like a mirror
	specularComponent;
	
}

