#version 330

in vec3 fragment_position;
in vec4 fragment_normal;
in vec2 texCoord1;

uniform sampler2D tex;
vec2 tc; 

void main()
{
    vec4 textureColor = texture(tex, texCoord1);

    float ambientStrength = 0.01;
    vec3 lightColor = vec3(0.5, 0.5, 0.7);

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fragment_normal.xyz);
    vec3 lightDir = normalize(vec3(0.0, -1.0, 0.0));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 0.3;

    vec3 result = (ambient + diffuse) * textureColor.rgb;

    gl_FragColor = vec4(result, textureColor.a);
}
