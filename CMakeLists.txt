# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (COSC_Lab_OpenGL2)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw-3.1.2/include/GLFW/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# OpenGL Lab04
add_executable(Part04
Part04/modelLoading.cpp
common/Shader.cpp
common/Shader.hpp
common/Camera.hpp
common/Camera.cpp
common/Scene.hpp
common/Scene.cpp
common/Object.hpp
common/Object.cpp
common/Triangle.hpp
common/Triangle.cpp
common/Quad.hpp
common/Quad.cpp
common/Mesh.hpp
common/Mesh.cpp
common/SimpleObjloader.hpp
common/SimpleObjloader.cpp
common/Controls.hpp
common/Controls.cpp
common/Texture.hpp
common/Texture.cpp
common/TextureShader.hpp
common/TextureShader.cpp

Part04/textureShader.frag
Part04/textureShader.vert
)
target_link_libraries(Part04
${ALL_LIBS}
assimp
)
set_target_properties(Part04 PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")
set_target_properties(Part04 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Part04/")
create_target_launcher(Part04 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Part04/")


# OpenGL Lab05
add_executable(Part05
    Part05/basicShading.cpp
    common/Shader.cpp
    common/Shader.hpp
    common/Camera.hpp
    common/Camera.cpp
    common/Scene.hpp
    common/Scene.cpp
    common/Object.hpp
    common/Object.cpp
    common/Triangle.hpp
    common/Triangle.cpp
    common/Quad.hpp
    common/Quad.cpp
    common/Texture.cpp
    common/Texture.hpp
    common/BasicMaterialShader.hpp
    common/BasicMaterialShader.cpp
    common/Mesh.hpp
    common/Mesh.cpp
    common/SimpleObjloader.hpp
    common/SimpleObjloader.cpp
    common/Controls.hpp
    common/Controls.cpp

    Part05/basicMaterialShader.frag
    Part05/basicMaterialShader.vert
)
target_link_libraries(Part05
${ALL_LIBS}
assimp
)
set_target_properties(Part05 PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")
set_target_properties(Part05 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Part05/")
create_target_launcher(Part05 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Part05/")



# OpenGL Lab06
add_executable(Part06
Part06/renderToTexture.cpp
common/Shader.hpp
common/Shader.cpp
common/Texture.cpp
common/Texture.hpp
common/Camera.hpp
common/Camera.cpp
common/Scene.hpp
common/Scene.cpp
common/Object.hpp
common/Object.cpp
common/BasicMaterialShader.hpp
common/BasicMaterialShader.cpp
common/Mesh.hpp
common/Mesh.cpp
common/Quad.hpp
common/Quad.cpp
common/SimpleObjloader.hpp
common/SimpleObjloader.cpp
common/Controls.hpp
common/Controls.cpp
common/PostProcessingShader.hpp
common/PostProcessingShader.cpp

Part06/basicMaterialShader.frag
Part06/basicMaterialShader.vert
Part06/Passthrough.vert
Part06/PostEffect.frag
)
target_link_libraries(Part06
${ALL_LIBS}
assimp
)
set_target_properties(Part06 PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")
set_target_properties(Part06 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Part06/")
create_target_launcher(Part06 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Part06/")


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
TARGET Part04 POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Part04${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/Part04/"
)
add_custom_command(
TARGET Part05 POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Part05${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/Part05/"
)

add_custom_command(
TARGET Part06 POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Part06${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/Part06/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

