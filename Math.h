#ifndef MATH_H
#define MATH_H

#include "Definitions.h"

class Math
{
public:
	Math();
	~Math();

	glm::mat4 lookAt(glm::vec3 lookAtPoint, glm::vec3 origin, glm::vec3 objectFacing);
};

#endif