#include "stdafx.h"
#include "ScnMger.h"
#include "Global.h"

ScnMger::ScnMger()
{
	mRenderer = NULL;

	//Init Objects List
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		mObj[i] = NULL;
	}

	//Create Hero Object
	mObj[HERO_ID] = new Object();
	mObj[HERO_ID]->SetPos(0, 0, 1);
	mObj[HERO_ID]->SetSize(1, 1);
	mObj[HERO_ID]->SetMass(1.f);
	mObj[HERO_ID]->SetCol(1, 1, 1, 1);
	mObj[HERO_ID]->SetVel(0, 0);
	mObj[HERO_ID]->SetAcc(0, 0);
	mObj[HERO_ID]->SetCoefFric(0.5f);
	mObj[HERO_ID]->SetKind(KIND_HERO);

	AddObject(2, 0, 1, 1, 1, 0, 0);

	//Init Renderer
	mRenderer = new Renderer(500, 500);

	//Load Test Texture
	mTestTexture = mRenderer->CreatePngTexture("./Textures/images.png");
	mTexSeq = mRenderer->CreatePngTexture("./Textures/spriteAnim3.png");

	
}

ScnMger::~ScnMger()
{
	if (mRenderer) { delete mRenderer; mRenderer = NULL; }
	if (mObj) { delete mObj; mObj[HERO_ID] = NULL; }
}

void ScnMger::Update(float eTime)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if(mObj[i] != NULL)
		mObj[i]->Update(eTime);
	}
}

void ScnMger::DoCollisionTest()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (mObj[i] == NULL)
		{
			continue;
		}

		for (int j = 0; j < MAX_OBJECTS; j++)
		{
			if (mObj[j] == NULL)
			{
				continue;
			}

			if (i == j)
			{
				continue;
			}
			//i obj
			float pX, pY, pZ, sX, sY;
			float minX, minY, maxX, maxY;
			mObj[i]->GetPos(&pX, &pY, &pZ);
			mObj[i]->GetSize(&sX, &sY);
			minX = pX - sX / 2.f;
			maxX = pX + sX / 2.f;
			minY = pY - sY / 2.f;
			maxY = pY + sY / 2.f;

			//j obj
			float pX1, pY1, pZ1, sX1, sY1;
			float minX1, minY1, maxX1, maxY1;
			mObj[j]->GetPos(&pX1, &pY1, &pZ1);
			mObj[j]->GetSize(&sX1, &sY1);
			minX1 = pX1 - sX1 / 2.f;
			maxX1 = pX1 + sX1 / 2.f;
			minY1 = pY1 - sY1 / 2.f;
			maxY1 = pY1 + sY1 / 2.f;

			if (RRCollision(minX, minY, maxX, maxY,
				minX1, minY1, maxX1, maxY1))
			{
				std::cout << "Collision\n";
				mObj[i]->SetCol(1, 0, 0, 1);
			}
			else
			{
				mObj[i]->SetCol(0, 0, 0, 1);
			}
		}
	}
}

void ScnMger::GarbageCollector()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (mObj[i] != NULL && i != HERO_ID)
		{
			float x, y, z;
			mObj[i]->GetPos(&x, &y, &z);
			if (x > 2.5f || x <-2.5f || y >2.5f || y < -2.5f)
					DeleteObject(i);
		}	
	}
}

bool ScnMger::RRCollision(float minX, float minY, float maxX, float maxY,
	float minX1, float minY1, float maxX1, float maxY1)
{
	if (maxX < minX1)
		return false;
	if (maxX1 < minX)
		return false;
	if (maxY < minY1)
		return false;
	if (maxY1 < minY)
		return false;
	return true;
}

void ScnMger::ApplyForce(float x, float y, float eTime)
{
	mObj[HERO_ID]->ApplyForce(x, y, eTime);
}

int gSeq = 0;

void ScnMger::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//if (theta >= 360) theta = 0;
	//else theta += 0.2;
	//g_Renderer->DrawSolidRect(x + cos(theta * PI / 180) * r, y + sin(theta * PI / 180) * r, 0, theta / 10, 1, 0, 1, 1);

	//g_Renderer->DrawSolidRect(xx, yy, 0, 10, 1, 0, 1, 1);

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (mObj[i] != NULL)
		{
			float x, y, z, sizeX, sizeY, r, g, b, a;
			mObj[i]->GetPos(&x, &y, &z);
			mObj[i]->GetSize(&sizeX, &sizeY);
			mObj[i]->GetCol(&r, &g, &b, &a);

			//mRenderer->DrawSolidRect(x*100.f, y*100.f, 0.f, sizeX*100.f, sizeY*100.f, r, g, b, a);

			
			mRenderer->DrawTextureRectHeight(
			x*100.f + 50,
			y*100.f,
			0.f,
			sizeX*100.f,
			sizeY*100.f,
			r, g, b, a,
			mTestTexture,
			z*100.f
			);

			// 스프라이트 애니메이션
			/*int seqX, seqY;
			seqX = gSeq % 7;
			seqY = (int)(gSeq / 7);

			gSeq++;
			if (gSeq > 35)
				gSeq = 0;

			mRenderer->DrawTextureRectSeqXY(
				x*100.f,
				y*100.f,
				0.f,
				sizeX*100.f,
				sizeY*100.f,
				r, g, b, a,
				mTexSeq,
				seqX, seqY,
				7, 2
			);*/
		}
	}
}

void ScnMger::AddObject(float pX, float pY, float pZ, float sX, float sY, float vX, float vY)
{
	int id = FindEmptyObjectSlot();
	if (id < 0)
	{
		return;
	}
	
	mObj[id] = new Object();
	mObj[id]->SetPos(pX, pY, pZ);
	mObj[id]->SetSize(sX, sY);
	mObj[id]->SetMass(1.f);
	mObj[id]->SetCol(1, 1, 1, 1);
	mObj[id]->SetVel(vX, vY);
	mObj[id]->SetAcc(0, 0);
	mObj[id]->SetCoefFric(0.5f);
	mObj[id]->SetKind(KIND_HERO);
}

int ScnMger::FindEmptyObjectSlot()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (mObj[i] == NULL)
			return i;
	}
	std::cout << "no more slot\n";
	return 1;
}

void ScnMger::DeleteObject(unsigned int id)
{
	if (mObj[id] != NULL)
	{
		delete mObj[id];
		mObj[id] = NULL;
	}
}

void ScnMger::Shoot(int shootID)
{
	if (shootID == SHOOT_NONE)
	{
		return;
	}
	float amount = 3.f;
	float bvX, bvY;
	bvX = 0.f;
	bvY = 0.f;

	switch (shootID)
	{
	case SHOOT_UP:
		bvX = 0.f;
		bvY = amount;
		break;
	case SHOOT_DOWN:
		bvX = 0.f;
		bvY = -amount;
		break;
	case SHOOT_LEFT:
		bvX = -amount;
		bvY = 0.f;
		break;
	case SHOOT_RIGHT:
		bvX = amount;
		bvY = 0.f;
		break;
	default:
		break;
	}

	float pX, pY, pZ;
	mObj[HERO_ID]->GetPos(&pX, &pY, &pZ);
	float vX, vY;
	mObj[HERO_ID]->GetVel(&vX, &vY);

	bvX = bvX + vX;
	bvY = bvY + vY;

	AddObject(pX, pY, pZ, 0.2, 0.2, bvX, bvY);
}