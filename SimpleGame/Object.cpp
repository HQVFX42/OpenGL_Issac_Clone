#include "stdafx.h"
#include "Object.h"
#include <math.h>
#include <float.h>

Object::Object()
{
}


Object::~Object()
{
}

void Object::Update(float eTime)
{
	//단위벡터구하기(정규화) 항상 반대방향으로 마찰력 주기 위함
	float magVel = sqrtf(mVelX*mVelX + mVelY * mVelY);
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
	}
	else
	{
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
	}

	//Calc velocity
	mVelX = mVelX + mAccX * eTime;
	mVelY = mVelY + mAccY * eTime;
	mVelZ = mVelZ + mAccZ * eTime;

	//Calc position
	mPosX = mPosX + mVelX * eTime;
	mPosY = mPosY + mVelY * eTime;
	mPosZ = mPosZ + mVelZ * eTime;
	
	//Calc friction
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