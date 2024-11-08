#version 330

attribute vec4 vtxPos;
attribute vec4 vtxNormal;
attribute vec2 texCoord;

uniform mat4 modelViewProjMat;

out vec3 fragment_position;
out vec4 fragment_normal;
out vec2 texCoord1;

void main()
{
    gl_Position = modelViewProjMat * vtxPos;

    fragment_position = vec3(gl_Position);

    fragment_normal = modelViewProjMat * vtxNormal;

    texCoord1 = texCoord;

}
