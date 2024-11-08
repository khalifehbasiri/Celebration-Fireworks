#include "Fireworks.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Fireworks::Fireworks() {
}

Fireworks::~Fireworks() {

}

float randomFloat(float min, float max) {
    return min + static_cast<float>(rand() / (static_cast<float>(RAND_MAX / (max - min))));
}

int randomInt(int min, int max) {
    return min + static_cast<int>(rand() / (static_cast<int>(RAND_MAX / (max - min))));
}

// Function to generate vivid firework colors
Vector4f generateVividFireworkColor() {
    int choice = randomInt(0, 5); // Choose one of the predefined colors
    Vector4f fireworkColor;

    switch (choice) {
    case 0: // Red
        fireworkColor = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
        break;
    case 1: // Green
        fireworkColor = Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
        break;
    case 2: // Blue
        fireworkColor = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
        break;
    case 3: // Yellow
        fireworkColor = Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
        break;
    case 4: // Magenta
        fireworkColor = Vector4f(1.0f, 0.0f, 1.0f, 1.0f);
        break;
    case 5: // Cyan
        fireworkColor = Vector4f(0.0f, 1.0f, 1.0f, 1.0f);
        break;
    default: // Default to white
        fireworkColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    return fireworkColor;
}

void Fireworks::initialize() {
    int choice = randomFloat(0, 100);
    fireworkColor = generateVividFireworkColor();
    fireworkPosition = Vector3f(randomFloat(-500.0f, 500.0f), 1000, randomFloat(-500.0f, 0.0f));
    Vector3f initialVelocity = Vector3f(0.0f, launchVelocity, 0.0f);
    loadTextureImages("fireTex.png");
    for (int i = 0; i < 800; ++i) {
        Particle particle;
        particle.position = fireworkPosition;
        particle.velocity = initialVelocity;
        particle.lifetime = particle.initialLife;
        particle.color = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        //particle.size = 0.4f;
        particles.push_back(particle);
    }
}

void Fireworks::update(float deltaTime, int time) {
    Vector3f gravity(0, -50.8f, 0);
    if (hasExploded) {
        for (auto& particle : particles) {
            // Only apply post-explosion physics here.
            particle.color = fireworkColor;
            particle.velocity += gravity * deltaTime;
            particle.velocity *= (1.0f - particle.dragCoefficient * deltaTime);
            particle.position += particle.velocity * deltaTime;
            particle.lifetime -= deltaTime;

            //if ((particle.lastTrailUpdate + particle.trailUpdateTime) < deltaTime) {
            //    if (particle.trailPositions.size() > 4) {
            //        particle.trailPositions.pop_front(); // Remove the oldest position
            //    }
            //    particle.trailPositions.push_back(particle.position); // Add the current position
            //    particle.lastTrailUpdate = deltaTime;
            //}
        }
    }
    else {
        // Before the explosion, update particles and check for explosion condition.
        for (auto& particle : particles) {
            // Continue moving as one before the explosion.
            particle.velocity += gravity * deltaTime;

            particle.position += particle.velocity * deltaTime;
            particle.lifetime -= deltaTime;
            fireworkPosition = particle.position;

            // If any particle reaches the explosion height, trigger the explosion.
            if (particle.position.y >= explosionHeight) {
                explode();
                hasExploded = true; // Prevent further explosions.
                explosionTime = time;
                break; // No need to check other particles in this frame.
            }
        }
    }

    // Remove dead particles.
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.lifetime <= 0; }),
        particles.end());
}

void Fireworks::render(Shader shader, Camera cam, int time) {

    shader.useProgram(1);

    Matrix4f viewMat = cam.getViewMatrix(NULL);
    Matrix4f projMat = cam.getProjectionMatrix(NULL);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(fireVAO);

    glUniform1f(glGetUniformLocation(shader.getProgId(), "currentTime"), (float)time);
    glUniform1f(glGetUniformLocation(shader.getProgId(), "explosionTime"), (float)explosionTime);
    glUniform3f(glGetUniformLocation(shader.getProgId(), "flashColor"), 1, 1, 1);
    glUniform1f(glGetUniformLocation(shader.getProgId(), "flashDuration"), 60.0f);

    for (const auto& particle : particles) {
        Matrix4f transMat = Matrix4f::translation(particle.position);
        Matrix4f scaleMat = Matrix4f::scale(particle.size, particle.size, particle.size);
        Matrix4f modelMat = transMat * scaleMat;
        Matrix4f modelViewMat = viewMat * modelMat;
        Matrix4f modelViewProjMat = projMat * modelViewMat;

        shader.copyMatrixToShader(modelMat, "modelMat");
        shader.copyMatrixToShader(modelViewProjMat, "modelViewProjMat");

        glUniform1i(glGetUniformLocation(shader.getProgId(), "particleTex"), 0);
        glUniform4f(glGetUniformLocation(shader.getProgId(), "particleColor"), particle.color.x, particle.color.y, particle.color.z, particle.color.w);
        glUniform1f(glGetUniformLocation(shader.getProgId(), "lifeFactor"), particle.lifetime / particle.initialLife);
        glUniform3f(glGetUniformLocation(shader.getProgId(), "emissionColor"), particle.color.x, particle.color.y, particle.color.z);
        glUniform1f(glGetUniformLocation(shader.getProgId(), "emissionStrength"), 2.5f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, getTexHandle(particle));

        Vector3f lightPosition = particle.position;
        Vector3f lightColor = Vector3f(particle.color.x * 2, particle.color.y * 2, particle.color.z * 2);
        Vector3f cameraPosition = cam.getPosition();

        glUniform3fv(glGetUniformLocation(shader.getProgId(), "lightPos"), 1, &lightPosition.x);
        glUniform3fv(glGetUniformLocation(shader.getProgId(), "lightColor"), 1, &lightColor.x);
        glUniform3fv(glGetUniformLocation(shader.getProgId(), "viewPos"), 1, &cameraPosition.x);


        glDrawArrays(GL_TRIANGLES, 0, 6);

        //if (!particle.trailPositions.empty()) {
        //    glBindVertexArray(trailVAO); // Assume a VAO for trail data
        //    std::vector<float> trailVertices;
        //    for (auto& pos : particle.trailPositions) {
        //        trailVertices.push_back(pos.x);
        //        trailVertices.push_back(pos.y);
        //        trailVertices.push_back(pos.z);
        //    }
        //    glBindBuffer(GL_ARRAY_BUFFER, trailVBO); // Assume a VBO for trail data
        //    glBufferData(GL_ARRAY_BUFFER, trailVertices.size() * sizeof(float), trailVertices.data(), GL_DYNAMIC_DRAW);
        //    glEnableVertexAttribArray(0);
        //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        //    glDrawArrays(GL_LINE_STRIP, 0, particle.trailPositions.size());
        //    glBindVertexArray(0);
        //}
    }

    glBindVertexArray(0);
    glUseProgram(0);
}


Vector3f Fireworks::calculateParticleVelocity(int num) {
    float theta = randomFloat(0.0f, 2.0f * M_PI);
    float phi = randomFloat(0.0f, M_PI);
    float velocityMagnitude = randomFloat(minVelocity, maxVelocity);

    float vx = velocityMagnitude * sin(phi) * cos(theta);
    float vy = velocityMagnitude * sin(phi) * sin(theta);
    float vz = velocityMagnitude * cos(phi);

    return Vector3f(vx, vy, vz);
}

void Fireworks::explode() {
    for (auto& particle : particles) {
        particle.velocity = calculateParticleVelocity(0);
        //particle.color = Vector4f(1.0f, 0, 0, 0);
        particle.position = fireworkPosition;
    }
}


GLuint Fireworks::getTexHandle(Particle particle)
{
    return (particle.tex.getTexId());
}

std::vector<Particle> Fireworks::getParticles() {
    return particles;
}

GLuint Fireworks::loadTextureImages(char* texturePath) {
    int width, height, numChannels;
    GLenum err;
    unsigned char* image = stbi_load(texturePath, &width, &height, &numChannels, 0);
    if (!image) {
        std::cerr << "Failed to load texture from file: " << texturePath << std::endl;
        return 0;  // Return an error code or handle it appropriately
    }
    //std::cerr << "Successfully loaded texture from file." << std::endl;
    //std::cerr << "Image width: " << width << ", height: " << height << ", channels: " << numChannels << std::endl;

    glGenTextures(1, &textureID);
    if (textureID == 0) {
        std::cerr << "OpenGL failed to generate a texture object." << std::endl;
        stbi_image_free(image); // Free image memory if texture generation fails
        return 0;
    }
    //std::cerr << "Successfully generated texture object." << std::endl;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = GL_RGB;
    if (numChannels == 4) {
        format = GL_RGBA;
    }
    else if (numChannels == 3) {
        format = GL_RGB;
    }
    else if (numChannels == 1) {
        format = GL_RED;
    }
    else {
        std::cerr << "Unsupported number of channels: " << numChannels << std::endl;
        stbi_image_free(image);
        return 0;
    }

    //std::cerr << "Right before glTexImage2D" << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    if ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after glTexImage2D: " << err << std::endl;
    }

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after setting texture parameters: " << err << std::endl;
    }

    stbi_image_free(image);

    return textureID;
}