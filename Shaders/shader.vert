#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
//layout(location = 1) in vec3 color;

//layout(location = 0) out vec3 vColor;
layout(location = 0) out vec2 vTextureCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
    //vColor = color;
    vTextureCoords = textureCoords;
}
