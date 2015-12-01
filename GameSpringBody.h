#include "JellyPhysics/JellyPhysics.h"
#include "Renderer.h"

class GameSpringBody : public SpringBody
{
public:
	

	GameSpringBody(World* w, const ClosedShape& shape, float massPerPoint, 
				   float shapeSpringK, float shapeSpringDamp, 
				   float edgeSpringK, float edgeSpringDamp, 
				   const Vector2& pos, float angleinRadians, 
				   const Vector2& scale, bool kinematic) :
	SpringBody(w, shape, massPerPoint,shapeSpringK, shapeSpringDamp,edgeSpringK, edgeSpringDamp,pos,angleinRadians,scale, kinematic)
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

