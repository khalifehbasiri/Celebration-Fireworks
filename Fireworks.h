#pragma once
#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <vector>
#include <deque>
#include <GL/glew.h>
#include "camera.h"
#include "graphicsObject.h"
#include "Texture.h"

struct Particle {
    Vector3f position;
    Vector3f velocity;
    Vector4f color;
    float lifetime;
    float initialLife = 4.0f;
    bool isExploding;
    float dragCoefficient = 0.9f;
    Texture tex;
    float size = 1.0f;
    std::deque<Vector3f> trailPositions;
    float trailUpdateTime;
    float lastTrailUpdate;
};

class Fireworks : public GraphicsObject {
public:
    Fireworks();
    ~Fireworks();

    void initialize();
    void update(float deltaTime, int time);
    void render(Shader shader, Camera cam, int time);
    void explode();
    Vector3f calculateParticleVelocity(int num);
    GLuint loadTextureImages(char* texturePath);
    GLuint getTexHandle(Particle particle);
    std::vector<Particle> getParticles();

private:
    std::vector<Particle> particles;
    Vector3f explosionPosition;
    Vector3f fireworkPosition;
    float minVelocity = 250.0f;
    float maxVelocity = 290.0f;
    float explosionHeight = 320.0f;
    float launchVelocity = 360.0f;
    bool hasExploded = false;
    Vector4f fireworkColor;
    GLuint textureID;
    int explosionTime = 10;
    int currentTime;


};

#endif // FIREWORKS_H
