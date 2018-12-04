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
	mObj[HERO_ID]->SetPos(0.f, 0.f, 0.5f);
	mObj[HERO_ID]->SetSize(0.5f, 0.5f, 0.5f);
	mObj[HERO_ID]->SetMass(1.f);
	mObj[HERO_ID]->SetCol(1.f, 1.f, 1.f, 1.f);
	mObj[HERO_ID]->SetVel(0.f, 0.f);
	mObj[HERO_ID]->SetAcc(0.f, 0.f);
	mObj[HERO_ID]->SetCoefFric(0.5f);
	mObj[HERO_ID]->SetKind(KIND_HERO);
	mObj[HERO_ID]->SetHP(10000);

	//Test addobject test building
	AddObject(1.f, 0.f, 0.5f, 1.f, 1.f, 1.f, 0.f, 0.f, KIND_BUILDING, 10000);

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

		int collisionCount = 0;

		for (int j = i+1; j < MAX_OBJECTS; j++)
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
			float pX, pY, pZ, sX, sY, sZ;
			float minX, minY, maxX, maxY, minZ, maxZ;
			mObj[i]->GetPos(&pX, &pY, &pZ);
			mObj[i]->GetSize(&sX, &sY, &sZ);
			minX = pX - sX / 2.f;
			maxX = pX + sX / 2.f;
			minY = pY - sY / 2.f;
			maxY = pY + sY / 2.f;
			minZ = pZ - sZ / 2.f;
			maxZ = pZ + sZ / 2.f;

			//j obj
			float pX1, pY1, pZ1, sX1, sY1, sZ1;
			float minX1, minY1, maxX1, maxY1, minZ1, maxZ1;
			mObj[j]->GetPos(&pX1, &pY1, &pZ1);
			mObj[j]->GetSize(&sX1, &sY1, &sZ1);
			minX1 = pX1 - sX1 / 2.f;
			maxX1 = pX1 + sX1 / 2.f;
			minY1 = pY1 - sY1 / 2.f;
			maxY1 = pY1 + sY1 / 2.f;
			minZ1 = pZ1 - sZ1 / 2.f;
			maxZ1 = pZ1 + sZ1 / 2.f;
			
			if (BBCollision(minX, minY, minZ, maxX, maxY, maxZ,
				minX1, minY1, minZ1, maxX1, maxY1, maxZ1))
			{
				//std::cout << "Collision\n";
				collisionCount++;
				ProcessCollision(i, j);
			}
		}
		if (collisionCount > 0)
		{
			mObj[i]->SetCol(1, 0, 0, 1);
		}
		else
		{
			mObj[i]->SetCol(1, 1, 1, 1);
		}
	}
}

void ScnMger::ProcessCollision(int i, int j)
{
	if (mObj[i] == NULL || mObj[j] == NULL)
		return;

	Object *ob1 = mObj[i];
	Object *ob2 = mObj[j];
	int kind1, kind2;
	ob1->GetKind(&kind1);
	ob2->GetKind(&kind2);

	if (kind1 == KIND_BUILDING && kind2 == KIND_BULLET)
	{
		int hp1, hp2;
		ob1->GetHP(&hp1);	//building
		ob2->GetHP(&hp2);	//bullet
		hp1 = hp1 - hp2;	//apply damage
		hp2 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);
	}

	if (kind1 == KIND_BULLET && kind2 == KIND_BUILDING)
	{
		int hp1, hp2;
		ob1->GetHP(&hp1);	//building
		ob2->GetHP(&hp2);	//bullet
		hp2 = hp2 - hp1;	//apply damage
		hp1 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);
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

			int kind;
			mObj[i]->GetKind(&kind);

			int hp;
			mObj[i]->GetHP(&hp);

			float vx, vy, mag;
			mObj[i]->GetVel(&vx, &vy);
			mag = sqrtf(vx * vx + vy * vy);		//속도의 크기

			if (x > 2.5f || x <-2.5f || y >2.5f || y < -2.5f)
			{
				DeleteObject(i);
			}

			if (kind == KIND_BULLET || kind == KIND_BUILDING)
			{
				if(hp <= 0)
				{
					DeleteObject(i);
					continue;
				}
			}

			if (kind == KIND_BULLET)
			{
				if (mag < FLT_EPSILON) 
				{
					DeleteObject(i);
					continue;
				}
			}
		
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

bool ScnMger::BBCollision(float minX, float minY, float minZ, float maxX, float maxY, float maxZ,
	float minX1, float minY1, float minZ1, float maxX1, float maxY1, float maxZ1)
{
	if (maxX < minX1)
		return false;
	if (maxX1 < minX)
		return false;
	if (maxY < minY1)
		return false;
	if (maxY1 < minY)
		return false;
	if (maxZ < minZ1)
		return false;
	if (maxZ1 < minZ)
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
			float x, y, z, sizeX, sizeY, sizeZ, r, g, b, a;
			mObj[i]->GetPos(&x, &y, &z);
			mObj[i]->GetSize(&sizeX, &sizeY, &sizeZ);
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

void ScnMger::AddObject(float pX, float pY, float pZ, float sX, float sY, float sZ, float vX, float vY, int kind, int hp)
{
	int id = FindEmptyObjectSlot();
	if (id < 0)
	{
		return;
	}
	
	mObj[id] = new Object();
	mObj[id]->SetPos(pX, pY, pZ);
	mObj[id]->SetSize(sX, sY, sZ);
	mObj[id]->SetMass(1.f);
	mObj[id]->SetCol(1, 1, 1, 1);
	mObj[id]->SetVel(vX, vY);
	mObj[id]->SetAcc(0, 0);
	mObj[id]->SetCoefFric(0.2f);
	mObj[id]->SetKind(kind);
	mObj[id]->SetHP(hp);
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
	float amount = 6.f;	//Bullet Speed
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

	AddObject(pX, pY, pZ, 0.2f, 0.2f, 0.2f, bvX, bvY, KIND_BULLET, 20);
}