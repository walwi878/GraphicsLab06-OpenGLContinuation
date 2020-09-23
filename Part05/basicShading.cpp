/*
 * basicShading
 *
 * Adapted from http://opengl-tutorial.org
 * by Stefanie Zollmann
 *
 * Model loading with assimp library and shading using phong reflection and phong shading
 *
 */


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Include GLEW
#include <GL/glew.h>


// Include GLFW
#include <glfw3.h>
GLFWwindow* window;


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/Shader.hpp>
#include <common/Texture.hpp>
#include <common/Object.hpp>
#include <common/Scene.hpp>
#include <common/Triangle.hpp>
#include <common/BasicMaterialShader.hpp>
#include <common/Mesh.hpp>
#include <common/Controls.hpp>

#include <common/SimpleObjloader.hpp>


/* ---- Helper Functions  ------------------------------------------------------- */

/*
 *  initWindow
 *
 *  This is used to set up a simple window using GLFW.
 *  Returns true if sucessful otherwise false.
 */
bool initWindow(std::string windowName){
	
	// Initialise GLFW
	if( !glfwInit() ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return false;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
		// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, windowName.c_str(), NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	
	return true;
	
}



int main( void )
{
	
	initWindow("LAB - Part 05");
	glfwMakeContextCurrent(window);
	
		// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	
		// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
		// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024/2, 768/2);
	
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	

	//create a Vertex Array Object and set it as the current one
	//we will not go into detail here. but this can be used to optimise the performance by storing all of the state needed to supply vertex data
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	/*
	 * Create Scene
	 * With a object and texture loaded from loader
	 */
	
	// Read our .obj file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	bool res = loadAssImp("suzanne.obj", indices, indexed_vertices, indexed_uvs, indexed_normals, true);
	if(res){
		//create scene
		Scene* myScene = new Scene();
		 //create mesh
		Mesh* myGeom = new Mesh();
		myGeom->setVertices(indexed_vertices);
		myGeom->setUVs(indexed_uvs);
		myGeom->setNormals(indexed_normals);
		myGeom->setIndices(indices);
		BasicMaterialShader* shader = new BasicMaterialShader( "basicMaterialShader");
		Texture* texture = new Texture("uvmap.DDS");
		shader->setTexture(texture);
		myGeom->setShader(shader);
		myScene->addObject(myGeom);
		

		Camera* myCamera = new Camera();
		myCamera->setPosition(glm::vec3(0,0,5));
		Controls* myControls = new Controls(myCamera);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		//Render loop
		while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 ){// Clear the screen
			
			if( glfwGetKey(window, GLFW_KEY_1) ==GLFW_PRESS)
				shader->setLightColour(glm::vec3(1.0,0.0,0.0));
			else if( glfwGetKey(window, GLFW_KEY_2) ==GLFW_PRESS)
				shader->setLightColour(glm::vec3(1.0,0.0,1.0));
			else if( glfwGetKey(window, GLFW_KEY_3) ==GLFW_PRESS)
				shader->setLightColour(glm::vec3(0.0,1.0,0.0));
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clear the depth buffer!!!
			
			// update camera controls with mouse input
			myControls->update();
			myScene->render(myCamera);
			
			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
			
		}
	
		
		delete texture;
		delete myScene;
		delete myCamera;
	}
	glDeleteVertexArrays(1, &VertexArrayID);
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	
	return 0;
}

