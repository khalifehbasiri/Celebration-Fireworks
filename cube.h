#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "graphicsObject.h""
#include "camera.h"
#include "Texture.h"

class Cube : public GraphicsObject {
public:
	Cube();
	~Cube();

	void initialize(Vertices& vtx1, Indices& ind1);
	void render(Shader shader, Camera cam, int time);
	void update(float deltaTime);
	//private:
		//GLuint vao, vbo, ibo;
		//GLsizei numIndices;
		//Matrix4f modelMat;
};

#endif