#include "JellyPhysics/JellyPhysics.h"
#include "Renderer.h"


class FallingBody : public SpringBody
{
public:

	FallingBody(World* world,ClosedShape &shape,float massPerPoint,float edgeSpringK, float edgeSpringDamp, Vector2 pos, float angle, Vector2 scale):
	  SpringBody(world,shape,massPerPoint,edgeSpringK,edgeSpringDamp,pos,angle,scale,false)
	  {
		dragPoint = -1;
	  }

	void accumulateExternalForces()
	{
		SpringBody::accumulateExternalForces();

		for (unsigned int i = 0; i < mPointMasses.size(); i++)
			mPointMasses[i].Force += Vector2(0.0f, -9.8f * mPointMasses[i].Mass);
		
		if (dragPoint != -1)
			mPointMasses[dragPoint].Force += dragForce;

		dragPoint = -1;
	}

	void debugDraw()
	{
		Renderer::RenderGlobalShapeLine(mPointMasses,1,1,1);
	}
};