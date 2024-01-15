#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

// texture samplers
uniform vec3 cubeColor;
uniform float ambientLight;
uniform float diffuseLight;
uniform vec3 lightPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(diffuseLight, diffuseLight, diffuseLight);

    vec3 result = (vec3(ambientLight, ambientLight, ambientLight) + diffuse) * cubeColor;
    FragColor = vec4(result, 1.0);
}
