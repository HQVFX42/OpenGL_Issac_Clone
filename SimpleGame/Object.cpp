#include "stdafx.h"
#include "Object.h"
#include "Global.h"
#include <math.h>
#include <float.h>

Object::Object()
{
	mBulletCoolTime = .0f;
	mBulletTime = .15f;	//키 계속 누르고 있을 시 나가는 총알 갯수 조절
}


Object::~Object()
{
}



void Object::Update(float eTime)
{
	//단위벡터구하기(정규화) 항상 반대방향으로 마찰력 주기 위함
	float magVel = sqrtf(mVelX*mVelX + mVelY * mVelY + mVelZ * mVelZ);
	float velX = mVelX / magVel;
	float velY = mVelY / magVel;

	float fricX = -velX;
	float fricY = -velY;
	float friction = mCoefFric * mMass * 9.8;
	fricX *= friction;
	fricY *= friction;

	if (magVel < FLT_EPSILON)
	{
		mVelX = 0.f;
		mVelY = 0.f;
		mVelZ = 0.f;
	}
	else
	{
		//Calc friction
		float accX = fricX / mMass;
		float accY = fricY / mMass;

		float afterVelX = mVelX + accX * eTime;
		float afterVelY = mVelY + accY * eTime;

		if (afterVelX * mVelX < 0.f)
			mVelX = 0.f;
		else
			mVelX = afterVelX;
		if (afterVelY *mVelY < 0.f)
			mVelY = 0.f;
		else
			mVelY = afterVelY;

		//Gravity
		mVelZ = mVelZ - GRAVITY * eTime;
	}

	//Calc velocity
	mVelX = mVelX + mAccX * eTime;
	mVelY = mVelY + mAccY * eTime;
	mVelZ = mVelZ + mAccZ * eTime;

	//Calc position
	mPosX = mPosX + mVelX * eTime;
	mPosY = mPosY + mVelY * eTime;
	mPosZ = mPosZ + mVelZ * 0.1f;		//eTime하면 잘 안되서 하드코딩으로 0.1로 박아놓는다.

	if (mPosZ <= 0.5f)
	{
		mState = STATE_GROUND;
		mVelZ = 0;
		mPosZ = 0.5f;
	}
	else
	{
		mState = STATE_AIR;
	}
	
	mBulletCoolTime -= eTime;
	
	if (mBulletCoolTime <= 0.f)
	{
		mBulletCoolTime = -1.f;
	}
}

void Object::ApplyForce(float x, float y, float z, float eTime)
{
	mAccX = x / mMass;
	mAccY = y / mMass;
	mAccZ = z / mMass;

	mVelX = mVelX + mAccX * eTime;
	mVelY = mVelY + mAccY * eTime;
	mVelZ = mVelZ + mAccZ * eTime;

	mAccX = 0.f;
	mAccY = 0.f;
	mAccZ = 0.f;
}

void Object::InitBulletCoolTime()
{
	mBulletCoolTime = mBulletTime;
}

bool Object::Fire()
{
	if (mBulletCoolTime <= 0.f)
	{
		return true;
	}
	return false;
}

void Object::GetPos(float *x, float *y, float *z)
{
	*x = mPosX;
	*y = mPosY;
	*z = mPosZ;
}

void Object::SetPos(float x, float y, float z)
{
	mPosX = x;
	mPosY = y;
	mPosZ = z;
}

void Object::GetSize(float *x, float *y, float *z)
{
	*x = mSizeX;
	*y = mSizeY;
	*z = mSizeZ;
}

void Object::SetSize(float x, float y, float z)
{
	mSizeX = x;
	mSizeY = y;
	mSizeZ = z;
}

void Object::GetMass(float *x)
{
	*x = mMass;
}

void Object::SetMass(float x)
{
	mMass = x;
}


void Object::GetCol(float *r, float *g, float *b, float *a)
{
	*r = mColR;
	*g = mColG;
	*b = mColB;
	*a = mColA;
}

void Object::SetCol(float r, float g, float b, float a)
{
	mColR = r;
	mColG = g;
	mColB = b;
	mColA = a;
}

void Object::GetVel(float *x, float *y, float *z)
{
	*x = mVelX;
	*y = mVelY;
	*z = mVelZ;
}
void Object::SetVel(float x, float y, float z)
{
	mVelX = x;
	mVelY = y;
	mVelZ = z;
}

void Object::GetAcc(float *x, float *y, float *z)
{
	*x = mAccX;
	*y = mAccY;
	*z = mAccZ;
}
void Object::SetAcc(float x, float y, float z)
{
	mAccX = x;
	mAccY = y;
	mAccZ = z;
}

void Object::GetCoefFric(float *x)
{
	*x = mCoefFric;
	
}
void Object::SetCoefFric(float x)
{
	mCoefFric = x;
}

void Object::GetKind(int *kind)
{
	*kind = mKind;
}
void Object::SetKind(int kind)
{
	mKind = kind;
}

void Object::GetHP(int *hp)
{
	*hp = mHP;
}
void Object::SetHP(int hp)
{
	mHP = hp;
}

void Object::GetState(int *state)
{
	*state = mState;
}
void Object::SetState(int state)
{
	mState = state;
}