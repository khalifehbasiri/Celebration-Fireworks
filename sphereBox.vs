#version 330

//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
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




const float inRefractFactor = 1.0;           // Refracting ray factor of the in material (e.g., air)
const float outRefractFactor = 1.0;          // Refracting ray factor of the out material (e.g., air)
    
in vec4 vtxPos;                             // The vertex position (in the local space) from VBO
in vec3 vtxNormal;                          // The vertex normal (in the local space) from VBO

out vec3 fragmentPosition;                  // The fragment position output into the fragment shader
out vec3 fragmentNormal;                    // The fragment normal output into the fragment shader
out vec3 reflectionVector;
out vec3 refractVector;

uniform mat4 modelViewProjMat;
uniform mat4 modelViewMat;
uniform mat4 modelMat;
uniform vec3 camPos;

void main()
{
    vec4 transformedNormal = transpose(inverse(modelMat)) * vec4(vtxNormal, 1.0);
    vec3 normal = normalize(transformedNormal.xyz);
    vec3 viewVec = normalize(vtxPos.xyz - camPos);
    
    // Compute reflection vector
    reflectionVector = reflect(viewVec, normal);
    
    // Compute refraction vector with proper handling for total internal reflection
    vec3 refractVecAttempt = refract(viewVec, normal, inRefractFactor / outRefractFactor);
    refractVector = length(refractVecAttempt) > 0.0 ? refractVecAttempt : reflectionVector;
    
    // Compute fragment position
    fragmentPosition = (modelMat * vtxPos).xyz;
    
    // Compute fragment normal
    fragmentNormal = normal;
    
    // Final position of the vertex
    gl_Position = modelViewProjMat * vtxPos; 
}
