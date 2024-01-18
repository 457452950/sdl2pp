#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

// texture samplers
uniform vec3 cubeColor;
uniform float ambientLight;
uniform float diffuseLight;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool ambient_enable_;
uniform bool diffuse_enable_;
uniform bool specular_enable_;

float specularStrength = 0.5;

void main()
{
    /* 漫反射 */
    // 法向量
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(diffuseLight, diffuseLight, diffuseLight);

    /* 镜面反射 */
    // 视线方向
    vec3 viewDir = normalize(viewPos - FragPos);
    // 光反射方向
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(diffuseLight, diffuseLight, diffuseLight);

    /* 环境光 */
    vec3 ambient = vec3(ambientLight, ambientLight, ambientLight);

    if (!specular_enable_) {
        specular = vec3(0);
    }
    if (!diffuse_enable_) {
        diffuse = vec3(0);
    }
    if (!ambient_enable_) {
        ambient = vec3(0);
    }

    vec3 result = (ambient + diffuse + specular) * cubeColor;
    FragColor = vec4(result, 1.0);
}
