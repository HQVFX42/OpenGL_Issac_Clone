#pragma once

#include "Renderer.h"
#include "Object.h"
#include "Global.h"
#include "Sound.h"

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
	void ApplyForce(float x, float y, float z, float eTime);
	void RenderScene();
	void AddObject(float pX, float pY, float pZ, 
		float sX, float sY, float sZ,
		float vX, float vY, float vZ,
		int kind, int hp, int state);
	void DeleteObject(unsigned int id);
	int FindEmptyObjectSlot();
	void Shoot(int shootID, float eTime);
	void GarbageCollector();
	bool RRCollision(float minX, float minY, float maxX, float maxY,
		float minX1, float minY1, float maxX1, float maxY1);
	bool BBCollision(float minX, float minY, float minZ, float maxX, float maxY, float maxZ,
		float minX1, float minY1, float minZ1, float maxX1, float maxY1, float maxZ1);
	void ProcessCollision(int i, int j);

private:
	Object * mObj[MAX_OBJECTS];
	Renderer * mRenderer;
	Sound * mSound;
	
	GLuint mTestTexture = 0;
	GLuint mBGTexture = 0;
	GLuint mTexSeq = 0;
	GLuint mTestParticle = 0;

	int mSoundBG = 0;
	int mSoundFire = 0;
	int mSoundExplosion = 0;
};


