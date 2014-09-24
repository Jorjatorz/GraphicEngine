//constains all classes declarations and other things
//#define DEBUG_MESSAGES
//#define SHADER_MESSAGES //Display shader uniform messages (not found etc)


#include <Windows.h>

//external includes
//SLD
#include <SDL.h>
//FreeImage
#include <FreeImage.h>
//OPENGL
#include <gl\glew.h>
#include <gl\gl.h>
//GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\norm.hpp>
#include <glm\gtx\component_wise.hpp>
//ASSIMP
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <assimp/vector3.h>
//BULLET
#include <btBulletDynamicsCommon.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>
#include<BulletCollision\NarrowPhaseCollision\btRaycastCallback.h>
//FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
//Awesonium
#include <Awesomium\WebCore.h>
#include <Awesomium\BitmapSurface.h>
#include <Awesomium\STLHelpers.h>

//custom type for decimals, default: float
typedef float real;



//forward declarations of classes
//allow declaration of pointers to them
class Camera;
class CollisionObject;
class Entity;
class FrameBuffer;
class InputManager;
class Light;
class ListenerReciver;
class Material;
class Math;
//class MemberFunctionSlot; Can't put it here, template class
class Mesh;
class MovableObject;
class PhysicsManager;
class RayCast;
class Renderer;
class ResourceManager;
class RigidBody;
class Root;
class SceneManager;
class SceneNode;
class Shader;
class SlotResolver;
class Texture;
class Timer;
class UICallbackListener;
class UIDisplayer;
class UIDrawer;
class UITextureSurfaceFactory; //Derived from awesomium
class UIManager;
class UIWindow;
class Window;
class WorldEditor;

//Not included
//class Singleton
//class GameResource;