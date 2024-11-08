#pragma once


#ifndef TREE_HEADER
#define TREE_HEADER 1


#include "graphicsObject.h"
#include "camera.h"
#include <vector>
#include <cstdlib> 
#include <ctime> 

class Tree : public GraphicsObject
{
public:
	Tree();
	~Tree();
	int makeTree(float height, float base, Vertices& vtx, Indices& ind, Matrix4f parentTransform);
	int makeTree(float height, float base, float u0, float u1, float v0, float v1, Vertices& vtx, Indices& ind, Matrix4f parentTransform);
	int Tree::makeCylinder(float height, float base, Vertices& vtx, Indices& ind, Matrix4f parentTransform, float u0, float u1, float v0, float v1);
	void createLeaves(float branchHeight, Vertices& vtx, Indices& ind, Matrix4f branchTransform);
	int render(Shader shader, Camera cam, int time, GLuint texUnit);

private:
	static int Tree::makeCylinder(float height, float base, Vertices& vtx, Indices& ind, Matrix4f parentTransform);
};


#endif