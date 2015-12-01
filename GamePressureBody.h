#include "JellyPhysics/JellyPhysics.h"
#include "Renderer.h"

class GamePressureBody : public PressureBody
{
public:

	GamePressureBody(World* w, const ClosedShape& s, float mpp,float gasPressure, float shapeK, float shapeD,
		float edgeK, float edgeD,const Vector2& pos, float angleInRadians, const Vector2& scale,bool kinematic):
	PressureBody(w,s,mpp,gasPressure,shapeK,shapeD,edgeK,edgeD,pos,angleInRadians,scale,kinematic)
	{
		dragPoint = -1;
	}

	void accumulateExternalForces()
	{
		PressureBody::accumulateExternalForces();

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
