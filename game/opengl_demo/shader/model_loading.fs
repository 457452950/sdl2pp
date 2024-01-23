#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflection1;
    float shininess;
};

uniform Material material;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    vec3 c1 = texture(skybox, R).rgb;
    vec3 c2 = texture(material.texture_reflection1, TexCoords).rgb;

    vec3 c3 = texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 c = c1 * c2 + c3;

    FragColor = vec4(c, 1.0);
}
