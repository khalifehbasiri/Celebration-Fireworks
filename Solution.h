#pragma once
#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER



//=============================================================================
// solution.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. solution framework for assignments
//
//
// Description:
//--------------
// A simple framework for drawing objecs 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================




#include "GL/glew.h"
#include "GL/glut.h"

#include "Shader.h"
#include "sphere.h"
#include "cylinder.h"
#include "surface.h"
#include "camera.h"
#include "texture.h"
#include "SkyBox.h"
#include "GL/nuss_math.h"
#include "Tree.h"
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include "Fireworks.h"
#include "stb_image.h"
#include "Cube.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

/*****************************************************************************************/
// DEFINES

#define UPDATE_RENDERRED_OBJECTS 1000
#define FRAME_TIME 30


/************************************************************************************************/
// STRUCTURES


struct TreeInstance {
	Matrix4f modelMatrix;  // Stores the position, scale, and rotation of a tree
};



class Solution
{
public:
	Solution();
	~Solution();
	// initializing the opengl functions and windows
	int initOpenGL();


	// static callback function 
	static void renderCB();
	static void keyboardCB(unsigned char key, int x, int y);
	static void specialKeyboardCB(int key, int x, int y);
	static void winResizeCB(int width, int height);
	static void timerCB(int operation);

	static void setSolution(Solution * _sol);

	// generaL shader that will be used by all objects
	// initialization of the solution
	int initSolution();


	

private:
	Shader shader;
	Sphere testSphere1;
	Shader sphereShader;
	Texture sphereCubeTexture;
	Cube testCube;
	Shader cubeShader;
	Fireworks testFire;
	Shader fireShader;
	std::vector<Fireworks> fireworksCollection;
	float timeSinceLastFirework = 0.0f;
	float fireworkLaunchInterval = 1.0f;
	SkyBox skybox;
	int numFrames;
	static Solution *sol;
	Camera cam;

	// rendering flags
	float factor;			// speed factor;
    int plotWireFrame;		// plot in wireframe of filled polyogns == 1
	int refractFlag;		// refractFlag == 1 use refraction rays 


	Texture GroundTex;
	Texture TreeTex;

	Shader groundShader;
	Surface groundSurface;

	Shader treeShader;
	Tree myTree;

	std::vector<Tree> treeInstances;


	// rendering flags


	Texture tex1;			// texture handle
	Texture tex2;			// texture handle
	Texture tex3;			// texture handle

	int time;
	TimePoint lastFrameTime;
	float deltaTime = 0.0f;


	void render();
	void keyboard(unsigned char key, int x, int y);
	void specialKeyboard(int key, int x, int y);
	void winResize(int width, int height);
	int timer(int operation);

	int updateObjects(int numFrames);

	//int loadMaterials(Shader shader);
	int printOpenGLError(int errorCode);
};



#endif