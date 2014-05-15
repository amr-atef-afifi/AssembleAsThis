#pragma once
#include "Angel.h"
#include <glm/glm.hpp>

class Cube
{
	private:
	glm::vec4 color;
	GLuint pointsIndex;
	GLboolean base;
	GLboolean visited;
	
	public:
	Cube(void);
	Cube::Cube(glm::vec4 color, GLuint index, GLboolean base);
	~Cube(void);
	glm::vec4 getColor();
	GLuint getPointsIndex();
	GLboolean isBase();
	GLboolean isVisited();

	void setColor(glm::vec4);
	void setPointsIndex(GLuint);
	void setBase(GLboolean);
	void setVisited(GLboolean);
};

