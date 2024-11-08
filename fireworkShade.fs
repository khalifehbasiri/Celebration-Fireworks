#version 330 core

uniform vec4 particleColor;
uniform float lifeFactor;
uniform vec3 emissionColor;
uniform float emissionStrength;
uniform sampler2D particleTex;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform float explosionTime;  // Time of the explosion
uniform float currentTime;    // Current time in your animation or scene
uniform vec3 flashColor;      // Color of the initial flash
uniform float flashDuration;  // Duration of the flash

in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor; 

void main() {
    float alpha = clamp(lifeFactor, 0.0, 1.0);
    vec4 baseColor = particleColor * texture(particleTex, TexCoord);
    //baseColor.rgb *= alpha;
    baseColor.a *= alpha;
    
    if (baseColor.a < 0.1)
        discard;

    //float distance = length(lightPos - FragPos);
    vec3 norm = normalize(FragPos - lightPos);

    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.9 * distance + 0.032 * distance * distance);
    vec3 diffuse = max(dot(norm, lightColor), 0.0) * lightColor;
	
	float timeSinceExplosion = currentTime - explosionTime;
	float flashIntensity = (timeSinceExplosion < flashDuration) ? exp(-3.0 * timeSinceExplosion / flashDuration) : 0.0;
	vec3 flashEffect = flashColor * flashIntensity;

    vec3 finalColor = baseColor.rgb + emissionColor * emissionStrength + diffuse * attenuation + flashEffect;

    FragColor = vec4(finalColor, baseColor.a); 
}
