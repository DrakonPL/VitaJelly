#ifndef Renderer_H
#define Renderer_H

#include <vita2d.h>

#include "JellyPhysics/JellyPhysics.h"
#include "JellyPhysics/PointMass.h"
#include <vector>

using namespace JellyPhysics;

int scaleX = 20;
int scaleY = 20;

int moveX = 960 / 2;
int moveY = 544 / 2;

class Renderer
{
public :

	static void RenderGlobalShapeLine(std::vector<PointMass> &mPointMasses,float R,float G,float B)
    {
		for (unsigned int i = 0; i < mPointMasses.size(); i++)
        {
			if((i + 1) < mPointMasses.size())
			{
				int pointX1 =  (mPointMasses[i].Position.X * scaleX) + moveX;
				int pointX2 = (mPointMasses[i+1].Position.X * scaleX) + moveX;
				
				int pointY1 = (544 - (mPointMasses[i].Position.Y * scaleY)) - moveY;
				int pointY2 = (544 - (mPointMasses[i+1].Position.Y * scaleY)) - moveY;
				
				vita2d_draw_line(pointX1,pointY1, pointX2,pointY2, RGBA8((int)(R * 255.0f), (int)(G * 255.0f), (int)(B * 255.0f), 255));
			}		
        }
		
		int pointX1 = (mPointMasses[mPointMasses.size()-1].Position.X * scaleX) + moveX;
		int pointX2 = (mPointMasses[0].Position.X * scaleX) + moveX;
		
		int pointY1 = (544 - (mPointMasses[mPointMasses.size()-1].Position.Y * scaleY)) - moveY;
		int pointY2 = (544 - (mPointMasses[0].Position.Y * scaleY)) - moveY;
		
		vita2d_draw_line(pointX1,pointY1, pointX2,pointY2, RGBA8((int)(R * 255.0f), (int)(G * 255.0f), (int)(B * 255.0f), 255));
    }

	static void RenderLine(Vector2 start,Vector2 stop,float R,float G,float B)
    {
        int pointX1 = (start.X * scaleX) + moveX;
		int pointX2 = (stop.X * scaleX) + moveX;
		
		int pointY1 = (544 - (start.Y * scaleY)) - moveY;
		int pointY2 = (544 - (stop.Y * scaleY)) - moveY;
		
		vita2d_draw_line(pointX1,pointY1, pointX2,pointY2, RGBA8((int)(R * 255.0f), (int)(G * 255.0f), (int)(B * 255.0f), 255));
    }
	
	static Vector2 ScreenToWorld(int x,int y)
	{
		Vector2 worldVector;
		
		worldVector.X = (x - moveX) / scaleX ;
		worldVector.Y = (544 - (y + moveY) ) / scaleY;
		
		return worldVector;
	}
	
	static Vector2 WorldToScreen(int x,int y)
	{
		Vector2 worldVector;
		
		worldVector.X = (x * scaleX) + moveX;
		worldVector.Y = (544 - (y * scaleY)) - moveY;
		
		return worldVector;
	}
};


#endif