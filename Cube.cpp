#include "Cube.h"



Cube::Cube(void)
{
}

Cube::Cube(glm::vec4 color, GLuint index, GLboolean base)
{
	this->color = color;
	this->pointsIndex = index;
	this->base = base;
	this->visited = false;
}

Cube::~Cube(void)
{
}

glm::vec4 Cube::getColor(){
	return color;	
}

GLuint Cube::getPointsIndex(){
	return pointsIndex;
}

GLboolean Cube::isBase(){
	return base;
}

GLboolean Cube::isVisited(){
	return visited;
}

void Cube:: setColor(glm::vec4 color){
	this->color = color;
}

void Cube::setPointsIndex(GLuint index){
	this->pointsIndex = index;
}

void Cube::setBase(GLboolean base){
	this->base = base;
}

void Cube::setVisited(GLboolean visited){
	this->visited = visited;
}