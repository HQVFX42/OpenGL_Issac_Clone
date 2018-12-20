#include "stdafx.h"
#include "ScnMger.h"
#include "Global.h"

bool gRoomBoss = false;

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
	mObj[HERO_ID]->SetVel(0.f, 0.f, 0.f);
	mObj[HERO_ID]->SetAcc(0.f, 0.f, 0.f);
	mObj[HERO_ID]->SetCoefFric(0.5f);
	mObj[HERO_ID]->SetKind(KIND_HERO);
	mObj[HERO_ID]->SetHP(10000);

	//Create Boss Object
	mObj[BOSS_ID] = new Object();

	//Test addobject test building
	AddObject(-1.f, 0.f, 0.5f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, KIND_BUILDING, 10000, STATE_GROUND);
	AddObject(0.f, 0.f, 0.f, 10.f, 10.f, -10.f, 0.f, 0.f, 0.f, KIND_BG, 10000, STATE_GROUND);
	AddObject(4.5f, 0.f, 0.f, .5f, .5f, .5f, 0.f, 0.f, 0.f, KIND_DOOR, 10000, STATE_GROUND);

	//Init Renderer
	mRenderer = new Renderer(1000, 1000);

	//Load Test Texture
	mTexTest = mRenderer->CreatePngTexture("./Textures/콩빵이.png");
	mTexSeq = mRenderer->CreatePngTexture("./Textures/Head.png");
	mTexBG = mRenderer->CreatePngTexture("./Textures/images.png");
	mTexDoor = mRenderer->CreatePngTexture("./Textures/콩빵이.png");
	mTexMonsterBoss = mRenderer->CreatePngTexture("./Textures/MonsterBoss.png");

	//Init Sound
	mSound = new Sound();

	//Load Sound
	mSoundBG = mSound->CreateSound("./Sounds/BG.mp3");
	mSoundFire = mSound->CreateSound("./Sounds/shotgun.mp3");
	mSoundExplosion = mSound->CreateSound("./Sounds/Explosion.mp3");
	//mSound->PlaySound(mSoundBG, true, 3.f);

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
	if (gRoomBoss) BossCreate();
}

void ScnMger::DoCollisionTest()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (mObj[i] == NULL)
		{
			continue;
		}
		// 배경 충돌
		float x, y, z;
		int kind;
		mObj[i]->GetPos(&x, &y, &z);
		mObj[i]->GetKind(&kind);
		if (kind == KIND_HERO || kind == KIND_BULLET || kind == KIND_BULLET) 
		{
			if (x > 4.5f)
			{
				mObj[i]->SetPos(4.5, y, z);
			}
			if (x < -4.5f)
			{
				mObj[i]->SetPos(-4.5, y, z);
			}
			if (y > 4.5f)
			{
				mObj[i]->SetPos(x, 4.5, z);
			}
			if (y < -4.5f)
			{
				mObj[i]->SetPos(x, -4.5, z);
			}
		}
		
		// 충돌체크
		int collisionCount = 0;

		for (int j = i+1; j < MAX_OBJECTS; j++)
		{
			if (mObj[j] == NULL) { continue; }

			if (i == j) { continue; }
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

	// 보스방
	if (kind1 == KIND_HERO && kind2 == KIND_DOOR)
	{
		mTexBG = mRenderer->CreatePngTexture("./Textures/콩빵이.png");
		gRoomBoss = true;
		float x, y, z;
		ob1->GetPos(&x, &y, &z);
		ob2->GetPos(&x, &y, &z);

		if (x > 4.5f)
		{
			ob1->SetPos(-x + .5f, y, z);
			ob2->SetPos(-x + .5f, y, z);
		}
	}

	if (kind1 == KIND_BUILDING && kind2 == KIND_HERO)
	{
		int hp1, hp2;
		ob1->GetHP(&hp1);	//building
		ob2->GetHP(&hp2);	//bullet
		hp2 = hp2 - hp1;	//apply damage
		hp1 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);

		//mSound->PlaySound(mSoundExplosion, false, 3.f);
	}
	// 총알 데미지
	if (kind1 == KIND_BUILDING && kind2 == KIND_BULLET)
	{
		int hp1, hp2;
		ob1->GetHP(&hp1);	//building
		ob2->GetHP(&hp2);	//bullet
		hp1 = hp1 - hp2;	//apply damage
		hp2 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);

		//mSound->PlaySound(mSoundExplosion, false, 3.f);
	}
	// 총알 데미지
	if (kind1 == KIND_BULLET && kind2 == KIND_BUILDING)
	{
		int hp1, hp2;
		ob1->GetHP(&hp1);	//building
		ob2->GetHP(&hp2);	//bullet
		hp2 = hp2 - hp1;	//apply damage
		hp1 = 0;

		ob1->SetHP(hp1);
		ob2->SetHP(hp2);

		//mSound->PlaySound(mSoundExplosion, false, 3.f);
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

			float vx, vy, vz, mag;
			mObj[i]->GetVel(&vx, &vy, &vz);
			mag = sqrtf(vx*vx + vy*vy + vz*vz);		//속도의 크기

			if (x > 5.0f || x <-5.0f || y >5.0f || y < -5.0f)
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

void ScnMger::ApplyForce(float x, float y, float z, float eTime)
{
	int state;
	mObj[HERO_ID]->GetState(&state);
	if (state == STATE_AIR)
	{
		z = 1.f;
	}

	mObj[HERO_ID]->ApplyForce(x, y, z, eTime);
}

int gSeq = 0;
float gTime = 0;

void ScnMger::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.0f, .0f, .0f, .0f);

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (mObj[i] != NULL)
		{
			float x, y, z, sizeX, sizeY, sizeZ, r, g, b, a;
			int kind, hp;

			mObj[i]->GetPos(&x, &y, &z);
			mObj[i]->GetSize(&sizeX, &sizeY, &sizeZ);
			mObj[i]->GetCol(&r, &g, &b, &a);
			mObj[i]->GetKind(&kind);
			mObj[i]->GetHP(&hp);

			if (kind == KIND_BG)
			{
				mRenderer->DrawTextureRectDepth(
					x*100.f,
					y*100.f,
					0,
					sizeX*100.f,
					sizeY*100.f,
					r, g, b, a,
					mTexBG,
					z*100.f
				);
			}
			if (kind == KIND_DOOR)
			{
				mRenderer->DrawTextureRectDepth(
					x*100.f,
					y*100.f,
					0,
					sizeX*100.f,
					sizeY*100.f,
					r, g, b, a,
					mTexBG,
					z*100.f
				);
			}
			if (kind == KIND_BUILDING)
			{
				mRenderer->DrawTextureRectHeight(
					x*100.f,
					y*100.f,
					0,
					sizeX*100.f,
					sizeY*100.f,
					r, g, b, a,
					mTexMonsterBoss,
					z*100.f
				);

				mRenderer->DrawSolidRectGauge(
					x*100.f, y*100.f + z * 100.f + 50.f, 0,
					sizeX*100.f, 4,
					1, 0, 0, 1,
					z*100.f,
					hp / 10000.f
				);
			}
			if (kind == KIND_HERO)
			{
				mRenderer->DrawTextureRectHeight(
					x*100.f,
					y*100.f,
					0,
					sizeX*100.f,
					sizeY*100.f,
					r, g, b, a,
					mTexTest,
					z*100.f
				);

				mRenderer->DrawSolidRectGauge(
					x*100.f, y*100.f + z * 100.f, 0,
					sizeX*100.f, 4,
					1, 0, 0, 1,
					z*100.f,
					1.f
				);
			}
			if (kind == KIND_BULLET)
			{
				mRenderer->DrawTextureRectHeight(
					x*100.f,
					y*100.f,
					0,
					sizeX*100.f,
					sizeY*100.f,
					r, g, b, a,
					mTexTest,
					z*100.f
				);
			}
			
			// 스프라이트 애니메이션
			/*if (kind == KIND_BUILDING) 
			{
				int seqX, seqY;
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
					10, 1
				);
			}*/

		}
	}
	mRenderer->DrawParticleClimate(
		0, 0, 0,
		5,
		1, 1, 1, 1,
		-1.f, 0.1f,
		mTexSeq,
		1.f,
		gTime);

	gTime += 0.01f;
}

void ScnMger::AddObject(float pX, float pY, float pZ, float sX, float sY, float sZ, float vX, float vY, float vZ, int kind, int hp, int state)
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
	mObj[id]->SetCol(1.f, 1.f, 1.f, 1.f);
	mObj[id]->SetVel(vX, vY, vZ);
	mObj[id]->SetAcc(0.f, 0.f, 0.f);
	mObj[id]->SetCoefFric(0.5f);
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

void ScnMger::Shoot(int shootID, float eTime)
{

	if (shootID == SHOOT_NONE)
	{
		return;
	}

	if (!mObj[HERO_ID]->Fire())
	{
		return;
	}
	float amount = 10.f;	//Bullet Speed
	float bvX, bvY, bvZ;
	bvX = 0.f;
	bvY = 0.f;
	bvZ = 0.f;

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
	float vX, vY, vZ;
	mObj[HERO_ID]->GetVel(&vX, &vY, &vZ);

	bvX = bvX + vX;
	bvY = bvY + vY;
	bvZ = bvZ + vZ;
	
	AddObject(pX, pY, pZ, 0.2f, 0.2f, 0.2f, bvX, bvY, bvZ, KIND_BULLET, 100, STATE_AIR);
	mObj[HERO_ID]->InitBulletCoolTime();
	//mSound->PlaySound(mSoundFire, false, 3.f);
}

void ScnMger::BossCreate()
{
	AddObject(1.f, 1.f, 0.5f, 0.2f, 0.2f, 0.2f, .0f, .0f, .0f, KIND_BUILDING, 10000, STATE_GROUND);
	gRoomBoss = false;
}
void ScnMger::BossMove()
{
	/*float theta = 0;
	float PI = 3.141592;
	float r = 1.f;
	if (theta >= 360) theta = 0;
	else theta += 0.2;*/

	float pX, pY, pZ;
	mObj[HERO_ID]->GetPos(&pX, &pY, &pZ);
	float vX, vY, vZ;
	mObj[HERO_ID]->GetVel(&vX, &vY, &vZ);
	float amount = .5f;
	float bvX, bvY, bvZ;
	bvX = 0.f;
	bvY = 0.f;
	bvZ = 0.f;
	bvX = bvX + vX;
	bvY = bvY + vY;
	bvZ = bvZ + vZ;
	
	mObj[BOSS_ID]->ApplyForce(bvX, bvY, bvZ, 1);
}