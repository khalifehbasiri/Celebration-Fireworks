#version 330 core
layout(points) in; // Input from vertex shader
layout(triangle_strip, max_vertices = 16) out; // Output triangle strip with up to 16 vertices

in vec2 TexCoord[];
out vec2 geoTexCoord;

uniform mat4 modelViewProjMat; // From vertex shader
//uniform float time; // You might pass the elapsed time since start for dynamic effects

void main() {
    // Constants for the spark effect
    const int numSparks = 8; // Number of sparks per particle
    const float sparkDistance = 0.1; // Radial distance of sparks from the particle center

    // Emit the original particle first
    gl_Position = gl_in[0].gl_Position;
    geoTexCoord = TexCoord[0];
    EmitVertex();
    EndPrimitive();

    // Emit sparks around the particle
    for (int i = 0; i < numSparks; ++i) {
        float angle = 2.0 * 3.14159 * float(i) / float(numSparks);
        float xOffset = sparkDistance * cos(angle);
        float yOffset = sparkDistance * sin(angle);

        // Calculate spark position relative to the particle center
        vec4 offset = vec4(xOffset, yOffset, 0.0, 0.0);
        gl_Position = gl_in[0].gl_Position + modelViewProjMat * offset;
        geoTexCoord = TexCoord[0]; // Sparks use the same texture coord as the central particle
        EmitVertex();
        EndPrimitive();
    }
}
