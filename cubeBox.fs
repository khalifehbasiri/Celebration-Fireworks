#version 330 core

out vec4 FragColor;

void main() {
    vec3 color = vec3(1.0, 0.0, 0.0); // Red
    vec3 emission = vec3(4.2, 0.0, 0.0); // Slight red emission for glow effect
    vec3 finalColor = color + emission;

    FragColor = vec4(finalColor, 1.0);
}
