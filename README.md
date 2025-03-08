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
