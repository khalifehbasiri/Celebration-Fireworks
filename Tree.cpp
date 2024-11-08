
/*********************************************************************************/
// INCLUDE FILES

#include "Tree.h"
#include <iostream>
#include <vector>
#include <cmath>  
#include <cstdlib>  

using namespace std;

/*********************************************************************************/
// DEFINES
#define MIN_X_RANGE -1.0
#define MAX_X_RANGE 1.0

#define MIN_Z_RANGE -1.0
#define MAX_Z_RANGE 1.0

/*********************************************************************************/

Tree::Tree()
{
}


Tree::~Tree()
{
}

int Tree::makeTree(float height, float base, float u0, float u1, float v0, float v1, Vertices& vtx, Indices& ind, Matrix4f parentTransform) 
{
	if (height < 1.0) return 0; // Base case for recursion

	// Make the main trunk with texture coordinates
	makeCylinder(height, base, vtx, ind, parentTransform, u0, u1, v0, v1);

	// Recursion to make branches
	float newHeight = height * 0.7f;
	float newBase = base * 0.7f;
	for (int i = 0; i < 3; ++i) {
		float angle = static_cast<float>(rand()) / RAND_MAX * 50.0f + 20.0f;
		if (rand() % 2) angle = -angle;

		Vector3f axis(
			static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
			static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
			static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f
		);

		Matrix4f rotationMatrix = Matrix4f::rotateVector(axis, angle, 1);
		Matrix4f translationMatrix = Matrix4f::translation(0, height, 0);
		Matrix4f localTransform = parentTransform * translationMatrix * rotationMatrix;

		makeTree(newHeight, newBase, u0, u1, v0, v1, vtx, ind, localTransform); // Recursive call with texture coordinates
		createLeaves(newHeight, vtx, ind, localTransform);
	}

	return 0;
}


int Tree::makeTree(float height, float base, Vertices& vtx, Indices& ind, Matrix4f parentTransform)
{
	if (height < 1.0) return 0; // Base case for recursion

	// Make the main trunk
	makeCylinder(height, base, vtx, ind, parentTransform);

	// Recursion to make branches
	float newHeight = height * 0.7f;
	float newBase = base * 0.7f;
	for (int i = 0; i < 3; ++i) {
		// Random angle between 20 and 70 degrees
		float angle = static_cast<float>(rand()) / RAND_MAX * 50.0f + 20.0f;
		if (rand() % 2) angle = -angle; // Randomly choose if the angle should be negative

		// Create a random rotation axis for each branch
		Vector3f axis(
			static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f, 
			static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f, 
			static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f  
		);
		

		Matrix4f rotationMatrix = Matrix4f::rotateVector(axis, angle, 1); // Rotate around the random axis
		Matrix4f translationMatrix = Matrix4f::translation(0, height, 0); // Move to the top of the current cylinder
		Matrix4f localTransform = parentTransform * translationMatrix * rotationMatrix;

		makeTree(newHeight, newBase, vtx, ind, localTransform); 

		createLeaves(newHeight, vtx, ind, localTransform);
	}

	return 0;
}

int Tree::makeCylinder(float height, float base, Vertices& vtx, Indices& ind, Matrix4f parentTransform, float u0, float u1, float v0, float v1) {
	int slices = 20;
	float step = 2 * M_PI / slices;
	int start_index = vtx.size();

	// Calculate the height of each vertex
	float deltaV = (v1 - v0) / height;

	// Generate vertices for the top and bottom circles of the cylinder
	for (int i = 0; i <= slices; i++) {
		float angle = i * step;
		float u = u0 + (u1 - u0) * (i / (float)slices);
		float x = base * cos(angle);
		float z = base * sin(angle);

		// Apply transformation to the top and bottom circle vertices
		Vector4f topVertex = parentTransform * Vector4f(x, height, z, 1.0);
		Vector4f bottomVertex = parentTransform * Vector4f(x, 0, z, 1.0);

		// Add vertices with texture coordinates
		vtx.push_back(Vertex(topVertex, Vector3f(0, 1, 0), Vector4f(0.64, 0.16, 0.16, 1), Vector2f(u, v1)));
		vtx.push_back(Vertex(bottomVertex, Vector3f(0, 1, 0), Vector4f(0.64, 0.16, 0.16, 1), Vector2f(u, v0)));
	}

	// Create indices for the cylinder sides
	for (int i = 0; i < slices; i++) {
		int next = (i + 1) % (slices + 1);
		ind.push_back(start_index + i * 2);
		ind.push_back(start_index + next * 2);
		ind.push_back(start_index + i * 2 + 1);

		ind.push_back(start_index + next * 2);
		ind.push_back(start_index + next * 2 + 1);
		ind.push_back(start_index + i * 2 + 1);
	}

	return 0;
}


int Tree::makeCylinder(float height, float base, Vertices& vtx, Indices& ind, Matrix4f parentTransform)
{
	int slices = 20;
	float step = 2 * M_PI / slices;
	int start_index = vtx.size();

	// Generate vertices for the top and bottom circles of the cylinder
	for (int i = 0; i < slices; i++) {
		float angle = i * step;
		float x = base * cos(angle);
		float z = base * sin(angle);

		// Apply transformation to the top circle vertex
		Vector4f topVertex = parentTransform * Vector4f(x, height, z, 1.0);
		vtx.push_back(Vertex(topVertex, Vector4f(0.64, 0.16, 0.16, 1))); // Color and position

		// Apply transformation to the bottom circle vertex
		Vector4f bottomVertex = parentTransform * Vector4f(x, 0, z, 1.0);
		vtx.push_back(Vertex(bottomVertex, Vector4f(0.64, 0.16, 0.16, 1))); // Color and position
	}

	// Create indices for the cylinder sides
	for (int i = 0; i < slices; i++) {
		int next = (i + 1) % slices;
		ind.push_back(start_index + i);
		ind.push_back(start_index + next);
		ind.push_back(start_index + i + slices);

		ind.push_back(start_index + next);
		ind.push_back(start_index + next + slices);
		ind.push_back(start_index + i + slices);
	}

	return 0;
}

void Tree::createLeaves(float branchHeight, Vertices& vtx, Indices& ind, Matrix4f branchTransform) {
	float leafSize = 0.1f; // Base size of the leaves
    int numberOfLeaves = 10; // Number of leaves per branch

    for (int i = 0; i < numberOfLeaves; ++i) {
        // Random rotation around the branch axis to spread the leaves
        float angle = static_cast<float>(rand()) / RAND_MAX * 360.0f;
        Matrix4f rotationMatrix = Matrix4f::rotateVector(Vector3f(0, 1, 0), angle, 1);

        // Random translation along the branch to distribute leaves
        float displacement = static_cast<float>(rand()) / RAND_MAX * branchHeight;
        Matrix4f translationMatrix = Matrix4f::translation(0, displacement, 0);

        // Apply transformations
        Matrix4f leafTransform = branchTransform * translationMatrix * rotationMatrix;

        // Leaf geometry (simple quad)
        Vector4f vertices[4] = {
            Vector4f(-leafSize, 0, 0, 1.0),
            Vector4f(leafSize, 0, 0, 1.0),
            Vector4f(leafSize, leafSize, 0, 1.0),
            Vector4f(-leafSize, leafSize, 0, 1.0)
        };
        std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
        for (auto& vert : vertices) {
            vert = leafTransform * vert;
            vtx.push_back(Vertex(vert, Vector4f(0, 1, 0, 1))); // Set color to green
        }
        for (auto index : indices) {
            ind.push_back(index + vtx.size() - 4); // Adjust indices correctly
        }
    }
}


/*********************************************************************************/


int Tree::render(Shader shader, Camera cam, int time, GLuint texUnit)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix
	Matrix4f modelViewMat;	// view matrix
	Matrix4f modelViewProjMat;	// projection matrix

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// use the created shader
	shader.useProgram(1);


	shader.copyIntVectorToShader((int*)&texUnit, 1, 1, "tex");

	shader.copyIntVectorToShader(&time, 1, 1, "time");


	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;

	// set the orientation  - this is model space to model space transformation 
	// Add code here
	rotMat = Matrix4f::rotateRollPitchYaw(0, 0, 0, 1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;


	// move matrix to shader
	//shader.copyMatrixToShader(modelMat, "modelMat");


	// set the camera matrix
	modelViewMat = cam.getViewMatrix(NULL) * modelMat;	// get the camera view transformation
#if 0	// move matrix to shader
	shader.copyMatrixToShader(modelViewMat, "modelViewMat");
#endif

	// set the projection matrix
	modelViewProjMat = cam.getProjectionMatrix(NULL) * modelViewMat;
	// move matrix to shader
	shader.copyMatrixToShader(modelViewProjMat, "modelViewProjMat");

	// transfer the camera position
	Vector3f pos = cam.getPosition();
	shader.copyFloatVectorToShader((float*)&pos, 1, 3, "camPos");




	// bind the vao
	glBindVertexArray(vao);

	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	//glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// unbind the vao
	glBindVertexArray(0);

	return 0;
}