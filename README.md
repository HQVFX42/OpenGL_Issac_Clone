# OpenGL_Issac_Clone
- Issac clone project with OpenGL
- Play sound using FMod

## Textures & Particles
- [Renderer](./SimpleGame/Renderer.cpp)

## Manage objects
- delete objects when hp <= 0 or bullet mag <= 0
	```cpp
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
	```

- apply damage
    ```cpp
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

			mSound->PlaySound(mSoundHitBoss, false, 3.f);
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

			mSound->PlaySound(mSoundHitBoss, false, 3.f);
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

			mSound->PlaySound(mSoundHitBoss, false, 3.f);
		}
	}
	```
