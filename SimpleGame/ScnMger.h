#pragma once

#include "Renderer.h"
#include "Object.h"
#include "Global.h"

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class ScnMger
{
public:
	ScnMger();
	~ScnMger();

	void Update(float eTime);
	void DoCollisionTest();
	void ApplyForce(float x, float y, float eTime);
	void RenderScene();
	void AddObject(float pX, float pY, float pZ, float sX, float sY, float vX, float vY);
	void DeleteObject(unsigned int id);
	int FindEmptyObjectSlot();
	void Shoot(int shootID);
	void GarbageCollector();
	bool RRCollision(float minX, float minY, float maxX, float maxY,
		float minX1, float minY1, float maxX1, float maxY1);

private:
	Object * mObj[MAX_OBJECTS];
	Renderer * mRenderer;
	
	GLuint mTestTexture = 0;
	GLuint mTexSeq = 0;
};


