#version 330 core

layout(location = 0) in vec3 position; // Particle position attribute
layout(location = 1) in vec2 texCoord;

uniform mat4 modelViewProjMat; // Model-view-projection matrix
uniform mat4 modelMat;

out vec2 TexCoord;
out vec3 FragPos;

void main() {
    vec4 worldPosition = modelMat * vec4(position, 1.0);
    gl_Position = modelViewProjMat * worldPosition;
    TexCoord = texCoord;
    FragPos = vec3(worldPosition);
}
