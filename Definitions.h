//constains all classes declarations and other things
//#define DEBUG_MESSAGES


//external includes
#include <SDL.h>
#include <SDL_image.h>
#include <gl\glew.h>
#include <gl\gl.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <assimp/vector3.h>

//custom type for decimals, default: float
typedef float real;



//forward declarations of classes
//allow declaration of pointers to them
class Camera;
class Entity;
class InputManager;
class Material;
class Mesh;
class MovableObject;
class Renderer;
class ResourceManager;
class Root;
class SceneManager;
class SceneNode;
class Shader;
class Texture;
class Timer;
class Window;

//Not included
//class Singleton
//class GameResource;