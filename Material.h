#ifndef MATERIAL_H
#define MATERIAL_H

#include "Definitions.h"

#include <map>

class Material
{
public:
	Material(void);
	~Material(void);

	glm::vec3 mDiffuseColor;
	glm::vec3 mSpecularColor;
	glm::vec3 mShininess;

	typedef enum
	{
		diffuseTexture,
		normalTexture,
		specularTexture
	} textureTypesEnum;

	typedef std::map<Texture*, textureTypesEnum> tTextureMap;
};

#endif