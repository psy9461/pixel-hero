#pragma once

#include "Common.h"
#include "ProcData.h"
#include "Sprite.h"

void loadSpriteChar(Entity* e, int index);
void freeSpriteChar();

void loadSpriteProj(Projectile* proj, int index);
void freeSpriteProj();
// ---------------------------------
//   Entity
// ---------------------------------

struct Entity
{
	Sprite* spr;

	bool leftRight;  // false : right, true : left
	iPoint pos;
	int prevBe = -1;
	int index = 0;
	float delta = 0.0f;
	bool repeat = true;
	MethodAfter method;

	float scale = 1.0f;

	virtual void paint(float dt, iPoint off) = 0;
};

// ---------------------------------
//   Projectile
// ---------------------------------

struct Projectile : Entity
{
	bool gen;
	bool hit;
	float speed;
	int ap;

	Projectile(int index);
	virtual ~Projectile();

	virtual void paint(float dt, iPoint off);
};


// ---------------------------------
//   Character
// ---------------------------------

struct Character : Entity
{
	bool alive;
	int maxHp;
	int hp;
	int ap;
	float speedMove;
	float speedAttack;
	int kind;
	int bottomMax;
	int topMax;
	int leftMax;
	int rightMax;
	bool downJump = false;



	float speedFall;
	uint8 jumpCnt = 0;
	float speedJump;

	iRect rtCol = iRectOne;
	iRect atkCol = iRectOne;
	
	float atkDelta, _atkDelta;

	virtual void paint(float dt, iPoint off);
	virtual void setDmg(float dmg);
	void jump();
};


// ---------------------------------
//   Hero
// ---------------------------------

void loadHero();
void drawHero(float dt, iPoint off);
void keyHero(float dt);
void freeHero();

enum HeroBehave
{
	HeroBehaveIdleTown = 0,
	HeroBehaveIdleField,
	HeroBehaveRunTown,
	HeroBehaveRunField,
	HeroBehaveSlide,
	HeroBehaveWallSlide,
	HeroBehaveSlideAir,
	HeroBehaveJump,
	HeroBehaveFall,
	HeroBehaveItem,
	HeroBehaveCastReady,
	HeroBehaveCastLoop,
	HeroBehaveSwordDraw,
	HeroBehaveSwordReturn,
	HeroBehaveAttack1,
	HeroBehaveAttack2,
	HeroBehaveAttack3,
	HeroBehaveAirattack1,
	HeroBehaveAirattack2,
	HeroBehaveAirattack3Ready,
	HeroBehaveAirattack3Loop,
	HeroBehaveAirattack3End,
	HeroBehaveHurt,
	HeroBehaveDie,

	HeroBehaveMax
};

struct Hero : Character
{
	HeroBehave be;
	int atkIndex;
	float atkDt, _atkDt;
	bool isAtk;

	Hero();
	virtual ~Hero();
	virtual void paint(float dt, iPoint off);
	void key(float dt, iPoint off);
	virtual void setDmg(float dmg);

	static void cbAttack(void* parm);
	static void cbJump(void* parm);
	static void cbHurt(void* parm);
	static void cbSlide(void* parm);
};

// ---------------------------------
//   Enemy
// ---------------------------------

void loadEnemy();
void drawEnemy(float dt, iPoint off);
void freeEnemy();

struct Enemy;

typedef void (*MethodAI)(Enemy* e, float dt);

enum EnemyBehave
{
	EnemyBehaveIdle = 0,
	EnemyBehaveRun,
	EnemyBehaveAttack1,
	EnemyBehaveAttack2,
	EnemyBehaveHurt,
	EnemyBehaveDead,

	EnemyBehaveMax
};
struct Enemy : Character
{
	float aiDt, _aiDt;
	EnemyBehave be;
	bool includeAttack;
	int randomBe;
	float randAiDt, _randAidt;

	MethodAI methodAI;
	iRect rangeDetect;
	iRect rangeAttack;

	virtual void paint(float dt, iPoint off);
	virtual void setDmg(float dmg);

//	static void cbRunReady(void* parm);
	static void cbAttack(void* parm);
	static void cbHurt(void* parm);
	static void cbDead(void* parm);
};


struct Walkable : Enemy
{
	Walkable(int index);
	virtual ~Walkable();

	virtual void paint(float dt, iPoint off);
};

void methodAI0(Enemy* e, float dt);
void methodAI1(Enemy* e, float dt);
void methodAI2(Enemy* e, float dt);
void methodAI3(Enemy* e, float dt);

void genProjectile(Enemy* e, bool dir, int ap);


