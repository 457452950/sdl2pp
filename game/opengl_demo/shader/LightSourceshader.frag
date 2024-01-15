#version 330 core
out vec4 FragColor;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    //    FragColor = vec4(Cor, 1);

    //    FragColor = texture(texture1, TexCoord);
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
