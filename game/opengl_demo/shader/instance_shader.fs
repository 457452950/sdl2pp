#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflection1;
    float shininess;
};

uniform Material material;

void main()
{

    vec3 c3 = texture(material.texture_diffuse1, TexCoords).rgb;

    FragColor = vec4(c3, 1.0);
}
