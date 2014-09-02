#include "UIObject.h"

#include "Shader.h"
#include "Texture.h"

UIObject::UIObject()
{
	generateBuffers();

	visible_ = true;
	movable_ = true;
	mSelected = false;

	//Transformations
	mPosition = mParentPosition = glm::vec3(0.0);
	mParentSize = glm::vec3(1.0, 1.0, 0.0);
	mSize = glm::vec3(1.0, 1.0, 0.0);
	mTransformMatrix = glm::mat4(1.0);

	//Rendering
	mColor = glm::vec4(0.1, 0.5, 0.9, 1.0);
	mTexture = NULL;
}


UIObject::~UIObject()
{
	glDeleteBuffers(1, &glVBO);
	glDeleteVertexArrays(1, &glVAO);
}

void UIObject::generateBuffers()
{
	//vertices
	const GLfloat vertex_positions[] =
	{
		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 0.0f,

		0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &glVAO);
	glBindVertexArray(glVAO);

	glGenBuffers(1, &glVBO);
	glBindBuffer(GL_ARRAY_BUFFER, glVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(Shader::VERTEXPOSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(Shader::VERTEXTEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void UIObject::drawObject(Shader* UIShader)
{
	if (visible_)
	{
		//update
		update();

		//Transformations
		setTransforms();

		//Uniforms
		sendUniforms(UIShader);


		glBindVertexArray(glVAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}
}

void UIObject::setTransforms()
{
	mTransformMatrix = glm::mat4(1.0);
	mTransformMatrix = glm::translate(mTransformMatrix, mPosition + mParentPosition);
	mTransformMatrix = glm::scale(mTransformMatrix, mSize * mParentSize);
}

void UIObject::setPosition(glm::vec2& pos)
{
	mPosition = glm::vec3(pos, 0.0);
}

void UIObject::setSize(glm::vec2& size)
{
	mSize = glm::vec3(size, 0.0);
}

void UIObject::sendUniforms(Shader* UIShader)
{
	UIShader->UniformMatrix("modelM", mTransformMatrix);
	UIShader->Uniform("UIColor", mColor);
	UIShader->Uniform("isTextured", isTextured()); //To check if its textured
	if (isTextured())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture->mTextureID);
		UIShader->UniformTexture("UITexture", 0);
	}
}

void UIObject::setColor(glm::vec3& newColor)
{
	if (isTextured())
	{
		mTexture = NULL;
	}

	mColor = glm::vec4(newColor, mColor.w);
}
void UIObject::setTexture(Texture* newTex)
{
	mTexture = newTex;
}
bool UIObject::isTextured()
{
	return mTexture == NULL ? false : true;
}

void UIObject::setSelected(bool sel)
{
	mSelected = sel;
}

bool UIObject::rayTestToObject(glm::vec2 rayCoords)
{
	//AABB
	glm::vec2 topVertices = glm::vec2(mPosition + (mSize / 2.0f)); //we divide size by 2 because the position is in the middle
	glm::vec2 lowVertices = glm::vec2(mPosition - (mSize / 2.0f)); //we divide size by 2 because the position is in the middle

	glm::vec2 resultTop = rayCoords - topVertices;
	glm::vec2 resultLow = rayCoords - lowVertices;

	//If we are inside the AABB
	if (glm::compMax(resultTop) < 0.0 && glm::compMin(resultLow) > 0.0)
	{
		return true;
	}

	return false;
}