#include "Math.h"


Math::Math()
{
}


Math::~Math()
{
}


glm::mat4 Math::lookAt(glm::vec3 lookAtPoint, glm::vec3 origin, glm::vec3 objectFacing)
{
	glm::vec3 targetVec = glm::normalize(lookAtPoint - origin);
	glm::vec3 mDir = objectFacing;
	glm::vec3 mAxis = glm::cross(targetVec, mDir);
	real angle = glm::acos(glm::dot(targetVec, mDir));

	if (mAxis != glm::vec3(0.0))
	{
		return glm::rotate(glm::mat4(1.0), -glm::degrees(angle), mAxis);
	}
	else
	{
		return glm::mat4(1.0); //Identity
	}

}