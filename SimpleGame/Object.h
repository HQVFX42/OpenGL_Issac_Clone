#pragma once

class Object
{
private:
	float mPosX, mPosY, mPosZ;
	float mColR, mColG, mColB, mColA;
	float mSizeX, mSizeY, mSizeZ;
	float mMass;
	float mVelX, mVelY;
	float mAccX, mAccY;
	float mCoefFric;

	int mKind;
	int mHP;

public:
	Object();
	~Object();

	void Update(float eTime);
	void ApplyForce(float x, float y, float eTime);

	void GetPos(float *x, float *y, float *z);
	void SetPos(float x, float y, float z);
	
	void GetSize(float *x, float *y, float *z);
	void SetSize(float x, float y, float z);

	void GetCol(float *r, float *g, float *b, float *a);
	void SetCol(float r, float g, float b, float a);

	void GetVel(float *x, float *y);
	void SetVel(float x, float y);

	void GetAcc(float *x, float *y);
	void SetAcc(float x, float y);

	void GetMass(float *x);
	void SetMass(float x);

	void GetCoefFric(float *x);
	void SetCoefFric(float x);

	void GetKind(int *kind);
	void SetKind(int kind);

	void GetHP(int *hp);
	void SetHP(int hp);
};

