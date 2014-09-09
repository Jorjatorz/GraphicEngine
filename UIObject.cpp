#include "UIObject.h"

#include "Shader.h"
#include "Texture.h"
#include "Root.h"
#include "UIManager.h"
#include "SceneManager.h"

UIObject::UIObject()
{
	generateBuffers();

	visible_ = true;
	movable_ = true;
	mSelected = false;
	mouseDown_ = false;

	//Transformations
	mPosition = mParentPosition = glm::vec2(0.0);
	mParentSize = glm::vec2(1.0);
	mSize = glm::vec2(1.0);
	mTransformMatrix = glm::mat4(1.0);

	//Rendering
	mColor = glm::vec4(0.1, 0.5, 0.9, 1.0);
	mTexture = NULL;

	//Text
	mText = "null";
	mTextColor = glm::vec3(1.0);
	mTextPosition = glm::vec2(0.0);
	mTextSize.x = 0.001;
	mTextSize.y = 0.002;

	//FreeType
	glActiveTexture(GL_TEXTURE0); //Create the bitmap (Maybe we could use texture class??)
	glGenTextures(1, &fontBitmap_);
	glBindTexture(GL_TEXTURE_2D, fontBitmap_);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

UIObject::~UIObject()
{
	glDeleteTextures(1, &fontBitmap_);
	glDeleteBuffers(1, &glVBO);
	glDeleteBuffers(1, &fontVBO_);
	glDeleteVertexArrays(1, &glVAO);
	glDeleteVertexArrays(1, &fontVAO_);
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

	//Quad
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

	//Font
	glGenVertexArrays(1, &fontVAO_);
	glBindVertexArray(fontVAO_);
	glGenBuffers(1, &fontVBO_);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO_);
	glVertexAttribPointer(Shader::VERTEXPOSITION, 4, GL_FLOAT, GL_FALSE, 0, 0); //Font made of quads - 4 vertex (1,2 position 3,4 texCoords)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

		//Render the quad
		glBindVertexArray(glVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//Render the font
		renderFont();

		//Rebind the shader
		mCurrentSceneManager->bindShader(mCurrentSceneManager->getShader("UIShader"));
		drawChildrens(UIShader);
	}
}

void UIObject::drawChildrens(Shader* UIShader)
{
	return;
}

void UIObject::setTransforms()
{
	mTransformMatrix = glm::mat4(1.0);
	mTransformMatrix = glm::translate(mTransformMatrix, glm::vec3(mPosition + mParentPosition, 0.0));
	mTransformMatrix = glm::scale(mTransformMatrix, glm::vec3(mSize * mParentSize, 0.0));
}

void UIObject::setPosition(glm::vec2& pos)
{
	mPosition = pos;
}

void UIObject::setSize(glm::vec2& size)
{
	mSize = size;
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
	glm::vec2 pos = mPosition + mParentPosition;
	glm::vec2 size = mSize * mParentSize;
	//AABB
	glm::vec2 topVertices = pos + (size / 2.0f); //we divide size by 2 because the position is in the middle
	glm::vec2 lowVertices = pos - (size / 2.0f); //we divide size by 2 because the position is in the middle

	glm::vec2 resultTop = rayCoords - topVertices;
	glm::vec2 resultLow = rayCoords - lowVertices;

	//If we are inside the AABB
	if (glm::compMax(resultTop) < 0.0 && glm::compMin(resultLow) > 0.0)
	{
		return true;
	}

	return false;
}

void UIObject::renderFont()
{
	Shader* textShader = mCurrentSceneManager->createShader("textRenderingShader", "textRendering");
	mCurrentSceneManager->bindShader(textShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontBitmap_);
	textShader->UniformTexture("fontBitmap", 0);
	textShader->Uniform("fontColor", mTextColor);


	FT_Face mFace = Root::getSingletonPtr()->mUIManager->mFreeTypeFace;
	FT_GlyphSlot g = mFace->glyph;

	//WE will increase x and y per char, so we store it
	real x = mTextPosition.x + mPosition.x + mParentPosition.x;
	real y = mTextPosition.y + mPosition.y + mParentPosition.y;

	glm::vec2 tSize = mTextSize;

	for (int i = 0; i < mText.size(); ++i) 
	{
		
		if (FT_Load_Char(mFace, mText.at(i), FT_LOAD_RENDER))
		{
			std::cout << "Cant load char: " << mText.at(i) << " in " << mName << std::endl;
		}

		glTexImage2D(GL_TEXTURE_2D,	0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		float x2 = x + g->bitmap_left * tSize.x;
		float y2 = -y - g->bitmap_top * tSize.y;
		float w = g->bitmap.width * tSize.x;
		float h = g->bitmap.rows * tSize.y;

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		glBindVertexArray(fontVAO_);
		glBindBuffer(GL_ARRAY_BUFFER, fontVBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		x += (g->advance.x >> 6) * tSize.x;
		y += (g->advance.y >> 6) * tSize.y;
	}
	
}