#version 330 core

layout (location = 0) in vec3 aPos;


uniform mat4 modelViewProjMat;

void main() {
    gl_Position = modelViewProjMat * vec4(aPos, 1.0);
}
