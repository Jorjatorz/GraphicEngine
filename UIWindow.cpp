#include "UIWindow.h"

#include "Root.h"
#include "UIManager.h"
#include "Shader.h"
#include "SceneManager.h"
#include "Texture.h"
#include "UIDrawer.h"


UIWindow::UIWindow(std::string name, real Width, real Height, std::string uiFilePath, SceneManager* manager)
{
	mName = name;
	mWidth = Width;
	mHeight = Height;
	mCurrentSceneManager = manager;

	mPosition = glm::vec2(0.0);
	glm::vec2 windowSize = mCurrentSceneManager->getWindowDimensions();
	mSize = glm::vec2(mWidth / windowSize.x, mHeight / windowSize.y);

	bResize = false;
	bMove = false;
	mLastMousePosition = glm::vec2(-1.0);

	//Create the quad
	createQuad();

	//Load a URL, window will delete it when delete the window
	mAwesomiumCore = Root::getSingletonPtr()->mUIManager->getUICore();

	mAwesomiumView = mAwesomiumCore->CreateWebView(mWidth, mHeight, 0, Awesomium::kWebViewType_Offscreen); //Offscreen
	Awesomium::WebURL url(Awesomium::WSLit(("file:///Data/UI/" + uiFilePath).c_str()));
	std::cout << "Loading UI: " << "Data\\UI\\" + uiFilePath << std::endl;
	mAwesomiumView->LoadURL(url);
	mAwesomiumView->SetTransparent(true);

	mCallbackListener.bind(mAwesomiumView);
	//Add resize callback by default
	mCallbackListener.addFunctionCallback("checkForResize", &UIWindow::checkForResize, this);
	mCallbackListener.addFunctionCallback("checkForMove", &UIWindow::checkForMove, this);;

	while (mAwesomiumView->IsLoading())
	{
		mAwesomiumCore->Update();
	}

	Awesomium::BitmapSurface* surface = (Awesomium::BitmapSurface*)mAwesomiumView->surface();
	if (surface == 0)
	{
		std::cout << "Error loading UI: " << "Data\\UI\\" + uiFilePath << std::endl;
	}
}


UIWindow::~UIWindow()
{
	mAwesomiumView->Destroy();
	glDeleteBuffers(1, &VBO_);
	glDeleteVertexArrays(1, &VAO_);
}

void UIWindow::createQuad()
{
	const GLfloat vertex_positions[] =
	{
		-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f,

		1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::VERTEXPOSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(Shader::VERTEXTEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void UIWindow::render(Shader* uiShad)
{
	//Check if we have to resize or move
	resize();
	moveWindow();

	glm::mat4 transMat = glm::mat4(1.0);
	transMat = glm::translate(transMat, glm::vec3(mPosition, 0.0));	
	transMat = glm::scale(transMat, glm::vec3(mSize, 0.0));

	//update awesomium texture - Just when the page change
	//updateAwesomiumTexture();

	//Send uniforms
	uiShad->UniformMatrix("modelM", transMat);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, static_cast<UIDrawer*>(mAwesomiumView->surface())->GetTexture()); //GetTexture updates the texture
	uiShad->UniformTexture("uiTexture", 0);

	//Render the quad
	glBindVertexArray(VAO_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

bool UIWindow::rayTestToQuad(glm::vec2 pointPos)
{
	glm::vec2 pos = mPosition;
	glm::vec2 size = mSize;
	//AABB
	glm::vec2 topVertices = pos + size; //we dont divide size by 2 because the size is 2
	glm::vec2 lowVertices = pos - size; //we dont divide size by 2 because the size is 2

	glm::vec2 resultTop = pointPos - topVertices;
	glm::vec2 resultLow = pointPos - lowVertices;

	//If we are inside the AABB
	if (glm::compMax(resultTop) < 0.0 && glm::compMin(resultLow) > 0.0)
	{
		return true;
	}

	return false;
}

void UIWindow::setMouseMove(glm::vec2 mousePos)
{
	glm::vec2 screenDimensions = mCurrentSceneManager->getWindowDimensions();
	glm::vec2 mWorldPosition;

	//Viewport formula - From NDC to normal device coords. i.e mPosition.y is negative because opengl has origin at bottom left corner, SDL_getMousePosition at top right corner
	mWorldPosition.x = (mPosition.x + 1) * screenDimensions.x * 0.5;
	mWorldPosition.y = (-mPosition.y + 1) * screenDimensions.y * 0.5;
	//Get the top left corner of the quad
	mWorldPosition.x -= mWidth / 2.0f;
	mWorldPosition.y -= mHeight / 2.0f;
	//Calculate the diference
	mWorldPosition -= mousePos;
	//Make it positive
	mWorldPosition = glm::abs(mWorldPosition);


	mAwesomiumView->Focus();
	mAwesomiumView->InjectMouseMove(mWorldPosition.x, mWorldPosition.y);
}

void UIWindow::setMouseButtonDown()
{
	mAwesomiumView->InjectMouseDown(Awesomium::kMouseButton_Left);
}

void UIWindow::setMouseButtonUp()
{
	mAwesomiumView->InjectMouseUp(Awesomium::kMouseButton_Left);
}

#include "InputManager.h"
void UIWindow::resize()
{
	if (bResize && InputManager::getSingletonPtr()->isMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if (mLastMousePosition == glm::vec2(-1.0))
		{
			mLastMousePosition = mCurrentSceneManager->getMousePosition_WindowSDL();
		}
		glm::vec2 screenDimensions = mCurrentSceneManager->getWindowDimensions();

		glm::vec2 mousePos = mCurrentSceneManager->getMousePosition_WindowSDL();

		glm::vec2 difference = mousePos - mLastMousePosition;

		mWidth += difference.x * 2.0f;
		mHeight += difference.y * 2.0f;

		mLastMousePosition = mousePos;

		mSize = glm::vec2(mWidth / screenDimensions.x, mHeight / screenDimensions.y);

		mAwesomiumView->Resize(mWidth, mHeight);
	}
	else if (!bMove)
	{
		bResize = false;
		mLastMousePosition = glm::vec2(-1.0);
	}
}

void UIWindow::checkForResize()
{
	bResize = true;
}

void UIWindow::checkForMove()
{
	bMove = true;
}

void UIWindow::moveWindow()
{
	if (bMove && InputManager::getSingletonPtr()->isMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if (mLastMousePosition == glm::vec2(-1.0))
		{
			mLastMousePosition = mCurrentSceneManager->getMousePosition_WindowSDL();
		}
		glm::vec2 screenDimensions = mCurrentSceneManager->getWindowDimensions();

		glm::vec2 mousePos = mCurrentSceneManager->getMousePosition_WindowSDL();

		glm::vec2 difference = mousePos - mLastMousePosition;

		difference /= screenDimensions;

		mPosition.x += difference.x * 2.0f;
		mPosition.y -= difference.y * 2.0f; //Invert the Y axis

		mLastMousePosition = mousePos;
	}
	else if (!bResize)
	{
		bMove = false;
		mLastMousePosition = glm::vec2(-1.0);
	}
}