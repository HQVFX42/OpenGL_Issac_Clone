/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#pragma comment(lib,"Winmm.lib")

#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "ScnMger.h"
#include "Global.h"

#include <math.h>



#define PI			3.141592
#define DIR_LEFT	0x01
#define DIR_RIGHT	0x02
#define DIR_UP		0x03
#define DIR_DOWN	0x04

//float x = 0, y = 0, r = 100, theta = 0;

ScnMger *gScnMger = NULL;

DWORD gPrevT = 0;

BOOL gKeyW, gKeyA, gKeyS, gKeyD, gKeySP = false;

int gShoot = SHOOT_NONE;

float gFX, gFY;

void RenderScene(void)
{
	//Calc ET
	if (gPrevT == 0) 
	{ 
		gPrevT = timeGetTime();
		return; 
	}
	DWORD CT = timeGetTime();
	DWORD ET = CT - gPrevT;
	gPrevT = CT;
	float eTime = (float)ET / 1000.f;

	std::cout << "ET : " << eTime << std::endl;

	float forceX = 0.f; 
	float forceY = 0.f;
	float forceZ = 0.f;
	float amount = 8.f;
	float zAmount = 100.f;

	if (gKeyW) { forceY += amount; }
	if (gKeyS) { forceY -= amount; }
	if (gKeyA) { forceX -= amount; }
	if (gKeyD) { forceX += amount; }
	if (gKeySP) { forceZ += zAmount; }

	gScnMger->ApplyForce(forceX, forceY, forceZ, eTime);
	gScnMger->Update(eTime);
	gScnMger->RenderScene();
	gScnMger->Shoot(gShoot, eTime);
	gScnMger->GarbageCollector();
	gScnMger->DoCollisionTest();

	std::cout << gKeyW << "\t" << gKeyS << "\t" << gKeyA << "\t" << gKeyD << "\t" << std::endl;
	
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

//void KeyInput(unsigned char key, int x, int y)
//{
//	/*static UCHAR pKeyBuffer[256];
//	DWORD dwDirection = 0; 
//
//	if (GetKeyboardState(pKeyBuffer))
//	{
//		if (pKeyBuffer[VK_LEFT] & 0xF0) xx-=1;
//		if (pKeyBuffer[VK_RIGHT] & 0xF0) xx+=1;
//		if (pKeyBuffer[VK_UP] & 0xF0) yy+=1;
//		if (pKeyBuffer[VK_DOWN] & 0xF0) yy-=1;
//	}*/
//
//	/*switch (key) {
//	case 'W':
//	case 'w':
//		float x, y;
//		g_obj->GetPos(&x, &y);
//		y += 0.5f;
//		g_obj->SetPos(x, y);
//		break;
//	case 'S':
//	case 's':
//		g_obj->GetPos(&x, &y);
//		y -= 0.5f;
//		g_obj->SetPos(x, y);
//		break;
//	case 'A':
//	case 'a':
//		g_obj->GetPos(&x, &y);
//		x -= 0.5f;
//		g_obj->SetPos(x, y);
//		break;
//	case 'D':
//	case 'd':
//		g_obj->GetPos(&x, &y);
//		x += 0.5f;
//		g_obj->SetPos(x, y);
//		break;
//	default:
//		break;
//	}*/
//
//	RenderScene();
//}

void KeyDownInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W') { gKeyW = TRUE; }
	if (key == 's' || key == 'S') { gKeyS = TRUE; }
	if (key == 'a' || key == 'A') { gKeyA = TRUE; }
	if (key == 'd' || key == 'D') { gKeyD = TRUE; }
	if (key == ' ') { gKeySP = TRUE; }
}

void KeyUpInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W') { gKeyW = FALSE; }
	if (key == 's' || key == 'S') { gKeyS = FALSE; }
	if (key == 'a' || key == 'A') { gKeyA = FALSE; }
	if (key == 'd' || key == 'D') { gKeyD = FALSE; }
	if (key == ' ') { gKeySP = FALSE; }
}

void SpecialKeyDownInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) { gShoot = SHOOT_UP; }
	if (key == GLUT_KEY_DOWN) { gShoot = SHOOT_DOWN; }
	if (key == GLUT_KEY_LEFT) { gShoot = SHOOT_LEFT; }
	if (key == GLUT_KEY_RIGHT) { gShoot = SHOOT_RIGHT; }
}

void SpecialKeyUpInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) { gShoot = SHOOT_NONE; }
	if (key == GLUT_KEY_DOWN) { gShoot = SHOOT_NONE; }
	if (key == GLUT_KEY_LEFT) { gShoot = SHOOT_NONE; }
	if (key == GLUT_KEY_RIGHT) { gShoot = SHOOT_NONE; }
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);		//키다운인풋에 계속 누르면 자동으로다운이 계속호출되는 설정을 꺼주는 함수
	glutSpecialUpFunc(SpecialKeyUpInput);
	

	gScnMger = new ScnMger();

	glutMainLoop();

	gScnMger = NULL;


    return 0;
}

