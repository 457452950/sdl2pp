#version 330 core
out vec4 FragColor;

// texture samplers
uniform vec3 inColor;
uniform vec3 inLight;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    //    FragColor = vec4(Cor, 1);

    //    FragColor = texture(texture1, TexCoord);
    FragColor = vec4(inColor * inLight, 1.0);
}
