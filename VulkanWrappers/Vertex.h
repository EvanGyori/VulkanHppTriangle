#pragma once

struct Vec2
{
    float x, y;
};

struct Vec3
{
    float x, y, z;
};

struct Vertex
{
    Vec3 position;

    //Vec3 color;

    Vec2 textureCoords;
};
