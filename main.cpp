#include <iostream>
#include <stdexcept>
#include <vector>

#include "Renderer.h"
#include "Vertex.h"

int main()
{
    std::vector<Vertex> vertices = {
	{
	    { 0.0f, -0.5f, 0.0f },
	    { 1.0f, 0.0f, 0.0f }
	},
	{
	    { -0.5f, 0.5f, 0.0f },
	    { 0.0f, 1.0f, 0.0f }
	},
	{
	    { 0.5f, 0.5f, 0.0f },
	    { 0.0f, 0.0f, 1.0f }
	},
    };

    try {
	Renderer renderer;
	while (renderer.isRunning()) {
	/*
	    static float foo = 0.0f;
	    foo += 0.01f;
	    for (int i = 0; i < 3; ++i)
		vertices[i].color = { foo, foo, foo };

	    if (foo >= 1.0f) {
		foo = 0.0f;
	    }
	*/

	    for (int i = 0; i < vertices.size(); ++i) {
		vertices[i].position.x += 0.001f;
	    }

	    renderer.draw(vertices);
	}
    } catch (std::runtime_error& error) {
	std::cout << error.what() << std::endl;
    }
}
