/*
 * renderToTexture.cpp
 *
 * Adapted from http://opengl-tutorial.org
 * by Stefanie Zollmann
 *
 * Demonstration of render to texture method - applies a post effect on the rendered scene
 *
 */


/* ------------------------------------------------------------------------- */
/* ---- INCLUDES ----------------------------------------------------------- */
/*
 * Include standard headers
 */
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
#include <common/Quad.hpp>
#include <common/BasicMaterialShader.hpp>
#include <common/Mesh.hpp>
#include <common/Controls.hpp>

#include <common/SimpleObjloader.hpp>
#include <common/PostProcessingShader.hpp>


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
	
	initWindow("LAB - Part 06");
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
	
 // We would expect width and height to be 1024 and 768
	int windowWidth = 1024;
	int windowHeight = 768;
	// But on MacOS X with a retina screen it'll be 1024*2 and 768*2, so we get the actual framebuffer size:
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	
	
	//create a Vertex Array Object and set it as the current one
	//we will not go into detail here. but this can be used to optimise the performance by storing all of the state needed to supply vertex data
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
    // ---------------------------------------------
    // Create Scene - With a object and texture loaded from loader
    // ---------------------------------------------
	
	// Read our .obj file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	bool res = loadAssImp("suzanne.obj", indices, indexed_vertices, indexed_uvs, indexed_normals, true);
	
	if(res){
		Scene* myScene = new Scene();
		 //create cube
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
		myCamera->setPosition(glm::vec3(0,0,3));
		Controls* myControls = new Controls(myCamera);
		
		
		// ---------------------------------------------
		// Render to Texture - specific code begins here
		// ---------------------------------------------

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLuint FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		
		// The texture we're going to render to
		GLuint renderedTexture;
		glGenTextures(1, &renderedTexture);
		
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		
		// Give an empty image to OpenGL ( the last "0" means "empty" )
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, windowWidth, windowHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
		
		// Poor filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
		

		// Always check that our framebuffer is ok
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		
		// the quad that we use to render the framebuffer texture to the screen
		Quad* outputQuad = new Quad();
		PostProcessingShader* postEffectShader = new PostProcessingShader("Passthrough.vert", "PostEffect.frag" );
		
		//Render loop
		while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 ){// Clear the screen
			
			/************************** First step: Render Scene into framebuffer *****************/
			// Render to our framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
			
			// Render to the screen
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glViewport(0,0,windowWidth,windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		
			//Render content
			// update camera controls with mouse input
			myControls->update();
			myScene->render(myCamera);
			
			/************************** Second step: Render texture containing the scene to the screen *****************/
			// Render to the screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// Render on the whole framebuffer, complete from the lower left corner to the upper right
			glViewport(0,0,windowWidth,windowHeight);
			
			// Clear the screen
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// Use our shader
			postEffectShader->bind();
			// Bind our texture in Texture Unit 0
			//the one from the famebuffer = render texture the one used in the shader texid
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderedTexture);
			// Set our "renderedTexture" sampler to user Texture Unit 0
			postEffectShader->bindTexture();
			postEffectShader->setTime((float)(glfwGetTime()*10.0f)); //set time to get animated effect in shader
			outputQuad->directRender(); //call render directly to render quad only without transformations

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
			
		}
		
		
		glDeleteVertexArrays(1, &VertexArrayID);
		delete texture;
		delete myScene;
		delete myCamera;
	}
    // Close OpenGL window and terminate GLFW
	glfwTerminate();
	
	return 0;
}

