

//=============================================================================
// solution.c
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



/*****************************************************************************/

// DEFINES

#include "Solution.h"

#include "GL/nuss_math.h"
#include <ctime>
#include "time.h"
#include <chrono>



#define NORMAL_SPEED 0.5f
#define MAX_SPEED 2.0f





/************************************************************/
// DEFINES
#define NO_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT  2
#define DIRECTIONAL_LIGHT 3


using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;


/************************************************************/
// GLOBALS


Solution *Solution::sol = NULL;

/****************************************************************************/

Solution::Solution() : numFrames(0)

{
	lastFrameTime = Clock::now();
}
 
/*************************************************************************/


Solution::~Solution()
{
	printf("\n exiting the progam gracefully\n");

}
/******************************************************************************/


// initializing the opengl functions and windows
int Solution::initOpenGL()
{
	//initialize OpenGL
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(300, 300);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Skybox");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glutDisplayFunc(Solution::renderCB);
	glutReshapeFunc(Solution::winResizeCB);
	glutKeyboardFunc(Solution::keyboardCB);
	glutSpecialFunc(Solution::specialKeyboardCB);
	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error - %s \n", glewGetErrorString(res));
		return (-1);
	}


	return 0;
}

/******************************************************************************/

// render callback function.  This is a static funcion


void Solution::renderCB()
{

	sol->render();
	
}


/************************************************************/

// keyboard callback function.  This is a static funcion


void Solution::keyboardCB(unsigned char key, int x, int y)
{
	sol->keyboard(key, x, y);
}


/************************************************************/

// special keyboard callback function.  This is a static funcion



void Solution::specialKeyboardCB(int key, int x, int y)
{
	sol->specialKeyboard(key, x, y);
}


/************************************************************/

// window resize callback function.  This is a static funcion



void Solution::winResizeCB(int width, int height)
{
	sol->winResize(width, height);
}

/************************************************************/

// timer  callback function.  This is a static funcion


void Solution::timerCB(int operation)
{

	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);	
	sol->timer(operation);

}


/************************************************************/

// timrt  function.  


int Solution::timer(int operation)
{

	switch (operation) {
	case UPDATE_RENDERRED_OBJECTS:
		updateObjects(numFrames);
		break;
	default:
		break;
	}
	return(0);
}




/******************************************************************************/
// initialization of the solution
int Solution::initSolution()
{
	int rc;
	Vertices vtx;
	Indices ind;
	char* texSky[6] = {
		"TropicalSunnyDayLeft2048.png",
		"TropicalSunnyDayRight2048.png",
		"TropicalSunnyDayUp2048.png",
		"TropicalSunnyDayDown2048.png",
		"TropicalSunnyDayFront2048.png",
		"TropicalSunnyDayBack2048.png" };
	char* nightSky[6] = {
		"right.png",
		"left.png",
		"up.png",
		"down.png",
		"front.png",
		"back.png" };
	char* fireTextureSprite = "fireTexture";


	float scalex = 200, scaley = 200, scalez = 200;

	const float minX = -2000.0f, maxX = 2000.0f;
	const float minZ = -2000.0f, maxZ = 2000.0f;
	const float minDistance = 200.0f;
	const int numberOfTrees = 60; // The number of trees you want to create

	// create a shader for the sphere
	rc = sphereShader.createShaderProgram("sphereBox.vs", "sphereBox.fs");
	if (rc != 0) {
		printf(" error after generating the spere shader \n");
		rc = 1;
	}
	int rc1 = cubeShader.createShaderProgram("cubeBox.vs", "cubeBox.fs");
	if (rc1 != 0) {
		printf(" error after generating the cube shader \n");
		rc1 = 1;
	}
	int rc2 = fireShader.createShaderProgram("fireworkShade.vs", "fireworkShade.fs");
	if (rc2 != 0) {
		printf(" error after generating the cube shader \n");
		rc2 = 1;
	}

	// create a surface
	// create a shader for the ground
	rc = groundShader.createShaderProgram("groundSurface.vs", "groundSurface.fs");
	if (rc != 0) {
		printf(" error after generating the wave surface shader \n");
		rc = 1;
	}

	Surface::createSurface(50, 50, 0, 1, 0, 1, vtx, ind);
	groundSurface.createVAO(groundShader, vtx, ind);
	groundSurface.setModelPosition(50, 0, 50);
	groundSurface.setModelScale(5000, 1, 5000);
	groundSurface.drawWave = 0;
	groundSurface.renderCircularWave = 0;

	// create a shader for the tree
	rc = treeShader.createShaderProgram("tree.vs", "tree.fs");
	if (rc != 0) {
		printf(" error after generating the tree shader \n");
		rc = 1;
	}
	treeInstances.reserve(numberOfTrees);
	for (int i = 0; i < numberOfTrees; ++i) {
		bool isTooClose;
		float posX, posZ;
		do {
			isTooClose = false;
			// Generate a random position
			posX = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
			posZ = minZ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxZ - minZ)));

			// Check the distance to other trees
			for (const auto& otherTree : treeInstances) {
				Vector3f otherPos = otherTree.getModelPosition();
				float dx = posX - otherPos.x;
				float dz = posZ - otherPos.z;
				float distanceSquared = dx * dx + dz * dz;
				if (distanceSquared < minDistance * minDistance) {
					isTooClose = true;
					break;
				}
			}
		} while (isTooClose);

		Tree tree;
		Vertices vtx;
		Indices ind;
		tree.makeTree(4, 0.2f, 0, 1, 0, 1, vtx, ind, Matrix4f::identity());

		tree.createVAO(treeShader, vtx, ind);
		tree.setModelPosition(posX, 0, posZ);
		tree.setModelScale(100, 100, 100);
		treeInstances.push_back(tree);
	}

	// load the texture of the surface
	GroundTex.loadTextureImage("ground.png", GL_TEXTURE_2D);
	TreeTex.loadTextureImage("tree.png", GL_TEXTURE_2D);


	testFire.initialize();
	testFire.createVAO(fireShader);
	testFire.setModelPosition(0, 0, 0);


	// set the camera initial position
	cam.setCamera(Vector3f(0, 150, 500), Vector3f(0, 400, 0), Vector3f(0, 1, 0));
	cam.setPerspectiveView(110, 1, 0.01f, 10000);


	//skybox initialization
	rc = skybox.init("skybox.vs", "skybox.fs");
	if (rc != 0) {
		printf(" error after generating the skybox  shader \n");
		rc = 1;
	}
	//skybox.loadColourTexture();
	//skybox.loadTextureImages(texSky);
	skybox.loadTextureImages(nightSky);


	plotWireFrame = 0;
	refractFlag = 0;

	time = 0;
	// speed factor
	factor = 1;

	err:
	return 0;
}


/**********************************************************************/

void Solution::setSolution(Solution * _sol)
{
	Solution::sol = _sol;
}



/************************************************************/

// render function.  


void Solution::render()
{

	time++;		// increament the time
	Vector3f viewerPosition = Vector3f(150, 150, 0);
	Vector3f lookAtPoint = Vector3f(150, 10, 100);
	Vector3f upVector = Vector3f(0, 1, 0);
	Matrix4f viewMat, projMat;
	

	viewerPosition = Vector3f(0, 500, 0);
	lookAtPoint = Vector3f(0, 0, 0);
	upVector = Vector3f(0, 0, 1);

	



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (!plotWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	skybox.render(cam);

	// render the skybox
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexHandle());

	// render the surface
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, GroundTex.getTexId());
	groundSurface.render(groundShader, cam, time, GL_TEXTURE4 - GL_TEXTURE0);


	glBindTexture(GL_TEXTURE_2D, TreeTex.getTexId());

	for (Tree& tree : treeInstances) {
		tree.render(treeShader, cam, time, GL_TEXTURE4 - GL_TEXTURE0);
	}

	for (auto& firework : fireworksCollection) {
		firework.render(fireShader, cam, time);
		for (auto& particle : firework.getParticles()) {
			glBindTexture(GL_TEXTURE_2D, firework.getTexHandle(particle));
		}
	}
	glutSwapBuffers();
}


/************************************************************/

// keyboard handling function. 


void Solution::keyboard(unsigned char key, int x, int y)
{
	static int nc = 0;
	nc++;
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		this->~Solution();
		exit(1);
		break;
	case 'x':	// not used
		break;
	case 'v':	// not used
		break;
		break;
	case 'w':
		cam.moveForward(NORMAL_SPEED*factor);
		break;
	case 's':
		cam.moveBackward(NORMAL_SPEED*factor);
		break;
	case 'k':
		cam.moveDown(NORMAL_SPEED * factor);
		break;
	case 'i':
		cam.moveUp(NORMAL_SPEED * factor);
		break;
	case 'a':
		cam.yaw((float) .2*factor);
		break;
	case 'd':
		cam.yaw((float)-.2*factor);
		break;
	case 'g':
		cam.moveRight(NORMAL_SPEED*factor);
		break;
	case 'G':
		cam.moveLeft(NORMAL_SPEED*factor);
		break;
	case 'z':
		cam.zoomIn();
		break;
	case 'Z':
		cam.zoomOut();
		break;
	case 't':
		refractFlag = (refractFlag == 0) ? 1 : 0;
		break;
	case 'p':
		plotWireFrame = plotWireFrame ? 0 : 1;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		factor = (float) (key - '0');
		break;
	}


}


/************************************************************/

// special keyboard handling  function.  



void Solution::specialKeyboard(int key, int x, int y)
{
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		exit(1);
		break;
	case GLUT_KEY_LEFT:
		cam.roll((float) .2*factor);
		break;
	case GLUT_KEY_UP:
		cam.pitch((float) .2*factor);
		break;
	case GLUT_KEY_RIGHT:
		cam.roll((float)-.2*factor);
		break;
	case GLUT_KEY_DOWN:
		cam.pitch((float)-.2*factor);
		break;
	}
}


/************************************************************/

// window resize handling function.  



void Solution::winResize(int width, int height)
{

	glViewport(0, 0, width, height);

}

/***********************************************************/
// update the state of the objects

int Solution::updateObjects(int numFrames)
{
	// recall that this will be carried out in the model space
		//testSphere1.incrementModelRotations(0, 0, 0.5);
	TimePoint currentTime = Clock::now();
	std::chrono::duration<float> duration = currentTime - lastFrameTime;
	deltaTime = duration.count();

	timeSinceLastFirework += deltaTime;

	if (timeSinceLastFirework >= fireworkLaunchInterval) {
		Fireworks newFirework;
		newFirework.initialize();
		fireworksCollection.push_back(newFirework);
		timeSinceLastFirework = 0.0f;
	}
	for (auto& firework : fireworksCollection) {
		firework.update(deltaTime, time);
	}

	lastFrameTime = currentTime;

	//testCube.update(deltaTime);
	//testFire.update(deltaTime);
	//lastFrameTime = currentTime;

	glutPostRedisplay();
	return 0;
}




/*************************************************************************************************************/

int Solution::printOpenGLError(int errorCode)
{
	switch (errorCode) {
	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
		break;
	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
		printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
		printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
		break;
	default:
		printf("openGL unknown error \n");
	}

	return 0;
}
