#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <time.h>

//psp2
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>
#include <vita2d.h>

//jelly physic
#include "JellyPhysics/JellyPhysics.h"
#include "StaticBody.h"
#include "FallingBody.h"
#include "GamePressureBody.h"
#include "GameSpringBody.h"


using namespace JellyPhysics;

extern unsigned int basicfont_size;
extern unsigned char basicfont[];

SceCtrlData     pad;
SceTouchData    touch;

int fxTouch;
int fyTouch;

#define lerp(value, from_max, to_max) ((((value*10) * (to_max*10))/(from_max*10))/10)
#define EXIT_COMBO (SCE_CTRL_START | SCE_CTRL_SELECT)
#define BLACK   RGBA8(  0,   0,   0, 255)
#define WHITE   RGBA8(255, 255, 255, 255)

//Jelly objects
World *mWorld;

//ground
StaticBody *groundBody;

//falling objects
ClosedShape fallingShape;
std::vector<FallingBody *> fallingBodies;
bool fall = true;

//pressure objects
ClosedShape pressureShape;
std::vector<GamePressureBody *> pressureBodies;
bool pressure = true;

//spring bodies
ClosedShape springShape;
std::vector<GameSpringBody *> springBodies;
bool spring = true;

//dragging
float dragX,dragY;
bool touchF = false;
int dragPoint = -1;
Body *dragBody;

//for fps
int fps;
long curTime = 0, lateTime = 0, count = 0, totalFrames = 0, beginTime = 0;
float msec = 0.0f, average = 0.0f;

void CalcFPS()
{
	count++;
	time_t seconds;
	time( &seconds );
	curTime = (long)seconds;
	if ( lateTime != curTime )
	{
		lateTime = curTime;
		msec = (1.0f / (float)count);
		totalFrames += count;
		average = (float)totalFrames / (float)(curTime - beginTime);
		fps = (int)count;
		count = 0;
	}
}

void init_stuff()
{
    //init time counter
	time_t begin;
	time( &begin );
	beginTime = (long)begin;

	//Init Jelly Objects
	mWorld = new World();

	//ground shape and floor
	ClosedShape groundShape;
	groundShape.begin();
	groundShape.addVertex(Vector2(-15.0f, -1.0f));
	groundShape.addVertex(Vector2(-15.0f, 1.0f));
	groundShape.addVertex(Vector2(15.0f, 1.0f));
	groundShape.addVertex(Vector2(15.0f, -1.0f));
	groundShape.finish();

	groundBody = new StaticBody(mWorld,groundShape,0,Vector2(0.0f, -10.0f),0,Vector2::One,false);
	
	//falling shape
	fallingShape.begin();
	fallingShape.addVertex(Vector2(-1.0f, 0.0f));
	fallingShape.addVertex(Vector2(0.0f, 1.0f));
	fallingShape.addVertex(Vector2(1.0f, 0.0f));
	fallingShape.addVertex(Vector2(0.0f, -1.0f));
	fallingShape.finish();
	
	//pressure shape
	pressureShape.begin();
	for (int i = 0; i < 360; i += 20)
	{
		pressureShape.addVertex(Vector2(cosf(VectorTools::degToRad((float)-i)), sinf(VectorTools::degToRad((float)-i))));
	}
	pressureShape.finish();
	
	//spring shape
	springShape.begin();
	springShape.addVertex(Vector2(-1.5f, 2.0f));
    springShape.addVertex(Vector2(-0.5f, 2.0f));
    springShape.addVertex(Vector2(0.5f, 2.0f));
    springShape.addVertex(Vector2(1.5f, 2.0f));
    springShape.addVertex(Vector2(1.5f, 1.0f));
    springShape.addVertex(Vector2(0.5f, 1.0f));
    springShape.addVertex(Vector2(0.5f, -1.0f));
    springShape.addVertex(Vector2(1.5f, -1.0f));
    springShape.addVertex(Vector2(1.5f, -2.0f));
    springShape.addVertex(Vector2(0.5f, -2.0f));
    springShape.addVertex(Vector2(-0.5f, -2.0f));
    springShape.addVertex(Vector2(-1.5f, -2.0f));
    springShape.addVertex(Vector2(-1.5f, -1.0f));
    springShape.addVertex(Vector2(-0.5f, -1.0f));
    springShape.addVertex(Vector2(-0.5f, 1.0f));
    springShape.addVertex(Vector2(-1.5f, 1.0f));
    springShape.finish();

	//important!!!!
	mWorld->setWorldLimits(Vector2(-100.0f,-100.0f),Vector2(100.0f,100.0f));
}

int main()
{
    vita2d_init();
    vita2d_set_clear_color(BLACK);
	
	init_stuff();

    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);

    vita2d_font *font = vita2d_load_font_mem(basicfont, basicfont_size);

    while (1) 
	{
		sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
		sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_OLED_OFF);

        sceCtrlPeekBufferPositive(0, &pad, 1);

        if (pad.buttons == EXIT_COMBO)
		{
            break;
        }
		
		//Update physic
		//1. method
		for (int i = 0; i < 4;i++)
			mWorld->update(1.0f/240.0f);


		//start drawing
        vita2d_start_drawing();
        vita2d_clear_screen();

		vita2d_font_draw_textf(font, 10, 10, WHITE, 25,"FPS: %4d  AVG: %3.1f  MS: %3f\n", fps, average, msec);	
        vita2d_font_draw_text(font, 650, 10, WHITE, 25, "Press Start + Select to exit");

		if (pad.buttons & SCE_CTRL_SQUARE)
		{
			if(fall)
			{
				FallingBody* body = new FallingBody(mWorld, fallingShape, 1.0f, 300.0f, 10.0f,Vector2(-2.0f, 5.0f), VectorTools::degToRad(rand()%360), Vector2::One);

				body->addInternalSpring(0, 2, 400.0f, 12.0f);
				body->addInternalSpring(1, 3, 400.0f, 12.0f);

				fallingBodies.push_back(body);
				fall = false;				
			}
        }else
		{
			fall = true;
		}

        if (pad.buttons & SCE_CTRL_CROSS) 
		{			
			if(pressure)
			{
				GamePressureBody * pressureBody = new GamePressureBody(mWorld,pressureShape, 1.0f, 70.0f, 10.0f, 1.0f, 300.0f, 20.0f, Vector2(2, 5), 0, Vector2(0.5f,0.5f),false);
				pressureBodies.push_back(pressureBody);
			
				pressure = false;
			}           
        }else
		{
			pressure = true;
		}

        if (pad.buttons & SCE_CTRL_CIRCLE)
		{
			if(spring)
			{
				GameSpringBody *springBody = new GameSpringBody(mWorld,springShape, 1.0f, 150.0f, 5.0f, 300.0f, 15.0f, Vector2(4, 5), 0.0f, Vector2::One,false);
				
				springBody->addInternalSpring(0, 14, 300.0f, 10.0f);
				springBody->addInternalSpring(1, 14, 300.0f, 10.0f);
				springBody->addInternalSpring(1, 15, 300.0f, 10.0f);
				springBody->addInternalSpring(1, 5, 300.0f, 10.0f);
				springBody->addInternalSpring(2, 14, 300.0f, 10.0f);
				springBody->addInternalSpring(2, 5, 300.0f, 10.0f);
				springBody->addInternalSpring(1, 5, 300.0f, 10.0f);
				springBody->addInternalSpring(14, 5, 300.0f, 10.0f);
				springBody->addInternalSpring(2, 4, 300.0f, 10.0f);
				springBody->addInternalSpring(3, 5, 300.0f, 10.0f);
				springBody->addInternalSpring(14, 6, 300.0f, 10.0f);
				springBody->addInternalSpring(5, 13, 300.0f, 10.0f);
				springBody->addInternalSpring(13, 6, 300.0f, 10.0f);
				springBody->addInternalSpring(12, 10, 300.0f, 10.0f);
				springBody->addInternalSpring(13, 11, 300.0f, 10.0f);
				springBody->addInternalSpring(13, 10, 300.0f, 10.0f);
				springBody->addInternalSpring(13, 9, 300.0f, 10.0f);
				springBody->addInternalSpring(6, 10, 300.0f, 10.0f);
				springBody->addInternalSpring(6, 9, 300.0f, 10.0f);
				springBody->addInternalSpring(6, 8, 300.0f, 10.0f);
				springBody->addInternalSpring(7, 9, 300.0f, 10.0f);
				
				springBodies.push_back(springBody);
				spring = false;
			}            
        }
		else
		{
			spring = true;
		}


        sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
        if (touch.reportNum > 0)
		{
            fxTouch = (lerp(touch.report[0].x, 1919, 960) - 50);
            fyTouch = (lerp(touch.report[0].y, 1087, 544) - 56);
			
			Vector2 dragVector = Renderer::ScreenToWorld((lerp(touch.report[0].x, 1919, 960) - 50),(lerp(touch.report[0].y, 1087, 544) - 56));
			
			dragX = dragVector.X;
			dragY = dragVector.Y;

			touchF = true;
        }else
		{
			touchF = false;
		}
		
		if (touchF == true)
		{
			if (dragBody != NULL)
			{
				PointMass *pm = dragBody->getPointMass(dragPoint);
				dragBody->setDragForce(VectorTools::calculateSpringForce(pm->Position, pm->Velocity, Vector2(dragX,dragY), Vector2(0,0), 0.0f, 100.0f, 10.0f), dragPoint);
			}
		}
		else
		{
			dragBody = NULL;
			dragPoint = -1;
		}

		if(touchF == true)
		{
			if(dragBody == NULL)
			{
				int body;
				mWorld->getClosestPointMass(Vector2(dragX,dragY), body, dragPoint);
				dragBody = mWorld->getBody(body);
			}
		}

		//draw ground
		groundBody->debugDraw();

		//bodies
		for(unsigned int i = 0; i < fallingBodies.size();i++)
			fallingBodies[i]->debugDraw();

		//pessure bodies
		for(unsigned int i = 0; i < pressureBodies.size();i++)
			pressureBodies[i]->debugDraw();

		//spring bodies
		for(unsigned int i = 0; i < springBodies.size();i++)
			springBodies[i]->debugDraw();
		
		//draw line from body to touch
		if(dragBody != NULL)
		{
			Renderer::RenderLine(dragBody->getPointMass(dragPoint)->Position,Vector2(dragX,dragY),1,1,1);
		}

        vita2d_end_drawing();
        vita2d_swap_buffers();
		
		CalcFPS();
    }

    vita2d_fini();
    vita2d_free_font(font);

    return 0;
}
