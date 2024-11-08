#version 330

in vec3 fragment_position;        // The fragment position interpolated from the vertex shader
in vec4 fragment_normal;          // The fragment normal interpolated from the vertex shader
in vec2 texCoord1;

uniform sampler2D tex;
vec2 tc; 

void main()
{
    // Sample the texture at the provided texture coordinates
    vec4 textureColor = texture(tex, texCoord1);

    // Enhance green color
    vec3 greenEnhancement = vec3(0.1, 0.5, 0.1); // Adjust these values to get the desired green shade
    textureColor.rgb *= greenEnhancement;

    // Reduced ambient lighting for nighttime
    float ambientStrength = 0.5; // Lower ambient light for a darker scene
    vec3 lightColor = vec3(0.5, 0.5, 0.7);  // Cool, dim light to simulate moonlight

    vec3 ambient = ambientStrength * lightColor;

    // Reduced diffuse lighting
    vec3 norm = normalize(fragment_normal.xyz);
    vec3 lightDir = normalize(vec3(0.0, -1.0, 0.0)); // example light direction, could be like moonlight from above
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 0.3; // Lower the diffuse to make it less intense

    // Combine the lighting influence
    vec3 result = (ambient + diffuse) * textureColor.rgb;

    // Set the output color of the pixel
    gl_FragColor = vec4(result, textureColor.a); // Use texture alpha
}
