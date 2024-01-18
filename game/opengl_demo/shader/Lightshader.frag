#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

uniform bool ambient_enable_;
uniform bool diffuse_enable_;
uniform bool specular_enable_;


void main()
{
    // 环境光
    vec3 ambient = vec3(0);
    if (ambient_enable_ == true) {
        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    }


    // 漫反射
    vec3 diffuse = vec3(0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    if (diffuse_enable_) {
        diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    }


    // 镜面光
    vec3 specular = vec3(0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    if (specular_enable_) {
        specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    }

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
