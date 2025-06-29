#version 430

layout(set = 0, binding = 0) uniform sampler2D textureSampler;

//layout(location = 0) in vec3 vColor;
layout(location = 0) in vec2 vTextureCoords;

layout(location = 0) out vec4 fColor;

void main()
{
    //fColor = vec4(vColor, 1.0);
    fColor = texture(textureSampler, vTextureCoords);
}
