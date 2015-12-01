#include "JellyPhysics/JellyPhysics.h"
#include "Renderer.h"


class StaticBody : public Body
{
public:

	StaticBody(World* w, const ClosedShape& shape, float massPerPoint, Vector2 position, float angleInRadians, Vector2 scale, bool kinematic):
	  Body(w,shape,massPerPoint,position,angleInRadians,scale,kinematic)
	  {

	  }

	void debugDraw()
	{
		Renderer::RenderGlobalShapeLine(mPointMasses,1,1,1);
	}
};

