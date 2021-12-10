#include "Entity.h"
#include "TileMap.h"

#include "iStd.h"

iPoint heroScale = iPointOne;

Sprite** _sprListChar = NULL;
Sprite** _sprListProj = NULL;

void loadSpriteChar(Entity* e, int index)
{
	if (_sprListChar == NULL)
	{
		_sprListChar = new Sprite * [MAX_CHAR];
		memset(_sprListChar, 0x00, sizeof(Sprite*) * MAX_CHAR);
	}
	if (_sprListChar[index] == NULL)
	{
		char path[512];
		sprintf(path, "assets/spritedata/%s.spr", _sprPathChar[index]);

		_sprListChar[index] = new Sprite(path);
	}

	e->spr = _sprListChar[index];
}

void freeSpriteChar()
{
	for (int i = 0; i < MAX_CHAR; i++)
	{
		if(_sprListChar[i])
			delete _sprListChar[i];
	}
	delete _sprListChar;
}

void loadSpriteProj(Projectile* proj, int index)
{
	if (_sprListProj == NULL)
	{
		_sprListProj = new Sprite * [MAX_PROJ];
		memset(_sprListProj, 0x00, sizeof(Sprite*) * MAX_PROJ);
	}
	if (_sprListProj[index] == NULL)
	{
		char path[512];
		sprintf(path, "assets/spritedata/%s.spr", _sprPathProj[index]);

		_sprListProj[index] = new Sprite(path);
	}

	proj->spr = _sprListProj[index];
}

void freeSpriteProj()
{
	for (int i = 0; i < MAX_PROJ; i++)
	{
		if (_sprListProj[i])
			delete _sprListProj[i];
	}
	delete _sprListProj;
}

// ---------------------------------
//   Projectile
// ---------------------------------
Projectile::Projectile(int index)
{
	loadSpriteProj(this, index);

	pos = iPointMake(-99999, -99999);
	gen = false;
	hit = false;
	ap = 0;
	speed = 270.0f;
	method = NULL;
}

Projectile::~Projectile()
{

}

bool moveEntity(float dt, iPoint& pos, bool leftRight, float speed, iSize sizeCol)
{
	bool result = false;

	if (leftRight == false)
	{
		float ex = pos.x - sizeCol.width / 2;
		int x = ex; x /= tileW;
		int y = pos.y; y /= tileH;

		float min = 0;
		for (int i = x; i > -1; i--)
		{
			Tile* tm = &tileMap[tileX * y + i];
			if (tm->value > -1 && tm->ti.pass == false)
			{
				min = tileW * (i + 1);
				break;
			}
		}

		ex -= speed * dt;
		if (ex < min)
		{
			ex = min;
			result = true;
		}
		pos.x = ex + sizeCol.width / 2;
	}
	else if (leftRight)
	{
		float ex = pos.x + sizeCol.width / 2;
		int x = ex; x /= tileW;
		int y = pos.y; y /= tileH;

		float max = tileW * maxTileX - 1;

		for (int i = x + 1; i < maxTileX; i++)
		{
			Tile* tm = &tileMap[tileX * y + i];
			if (tm->value > -1 && tm->ti.pass == false)
			{
				max = tileW * i - 1;
				break;
			}
		}
		ex += speed * dt;
		if (ex > max)
		{
			ex = max;
			result = true;
		}
		pos.x = ex - sizeCol.width / 2;
	}

	return result;
}

void Projectile::paint(float dt, iPoint off)
{
	// 방향은 쏜 방향으로;

	spr->paint(dt, pos + off, method, this, 0, index, delta, leftRight, 3.0f * scale, true);

	iRect rtDef = spr->getRtDef(0, index, pos + off, 3.0f * scale, leftRight);
	//setRGBA(1, 0, 0, 1);
	//drawRect(rtDef);
	//setRGBA(1, 1, 1, 1);

	hit = moveEntity(dt, pos, leftRight, speed, rtDef.size);

	iRect rtAtt = spr->getRtAtt(0, index, pos + off, scale, true);
	iRect heroRtDef = hero->spr->getRtDef(hero->be, hero->index, hero->pos + off, 3.0f * scale, hero->leftRight);

	if (containRect(rtAtt, heroRtDef) && hero->hp > 0)
	{
		hero->setDmg(ap);
		hit = true;
	}
}

// ---------------------------------
//   Character
// ---------------------------------

void Character::paint(float dt, iPoint off)
{
	// down
	int x = pos.x; x /= tileW;
	int y = pos.y; y /= tileH;
	bottomMax = tileH * tileY - 1;
	bool existBottom = false;

	int s, n;

	if (speedFall >= 0.0f)
	{
		for (int j = y + 1; j < tileY; j++)
		{
			s = pos.x - rtCol.size.width / 2; s /= tileW;
			n = pos.x + rtCol.size.width / 2; n /= tileW;
			n++;
			for (int i = s; i < n; i++)
			{
				Tile* tm = &tileMap[tileX * j + i];
				//if (tm->value > 0 && tm->ti.gt > 0)
				if (tm->value > -1 && tm->ti.gt > 0)
				{
					bottomMax = tileH * j - 1;
					existBottom = true;
					break;
				}
			}
			if (existBottom)
				break;
		}
	}

	// up
	int topY = pos.y - rtCol.size.height; topY /= tileH;
	topMax = 0;
	bool existTop = false;
	for (int j = topY; j > -1; j--)
	{
		s = pos.x - rtCol.size.width / 2; s /= tileW;
		n = pos.x + rtCol.size.width / 2; n /= tileW;
		n++;
		for (int i = s; i < n; i++)
		{
			Tile* tm = &tileMap[tileX * j + i];
			if (tm->value > -1 && (tm->ti.pass == false))
			{
				topMax = tileH * (j + 1);
				existTop = true;
				break;
			}
		}
		if (existTop)
			break;
	}

	if (pos.y < bottomMax && pos.y > topMax)
	{
		// 중력 받는중 (공중에 뜸)
		pos.y += speedFall * dt;// v.y;

		float posYTop = pos.y - rtCol.size.height;
		if (posYTop < topMax)
		{
			posYTop = topMax;
			pos.y = posYTop + rtCol.size.height;
			speedFall = 0;
			speedJump = 0;
		}

		if (pos.y > bottomMax && (speedFall + speedJump) > 0)
		{
			pos.y = bottomMax;
			jumpCnt = 0;
			speedFall = 0;
			speedJump = 0;
			downJump = false;
		}

		speedFall += gravityForce * dt;
		if (speedFall >= gravityMax)
			speedFall = gravityMax;
	}
}

void Character::jump()
{
	printf("jump\n");
	speedJump += jumpForce;
	jumpCnt++;
}

void Character::setDmg(float dmg)
{
	hp -= dmg;
}

// ---------------------------------
//   Hero
// ---------------------------------
void loadHero()
{
	hero = new Hero();
}

void freeHero()
{
	delete hero;
}

void drawHero(float dt, iPoint off)
{
	hero->paint(dt, off);
}

Hero::Hero()
{
	loadSpriteChar(this, MAX_CHAR - 1);

	be = HeroBehaveIdleField;
	atkIndex = 0;
	_atkDt = 1.0f;
	atkDt = 0.0f;
	isAtk = false;

	leftRight = true;

	alive = true;
	maxHp = 200;
	hp = maxHp;
	ap = 15;
	speedMove = 1.0f;
	speedAttack = 2.0f;
	speedFall = 0;
	speedJump = 0;
	rtCol.size = iSizeMake(30, 80);
	kind = MAX_CHAR - 1;

	method = NULL;
}

Hero::~Hero()
{
}

float slideDt = 0.0f;
float _slideDt = 1.5f;
float _cooldownSlide = 2.0f;
float cooldownSlide = _cooldownSlide;

void Hero::paint(float dt, iPoint off)
{
	Character::paint(dt, off);

	cooldownSlide += dt;
	if (cooldownSlide >= _cooldownSlide)
		cooldownSlide = _cooldownSlide;

	if (isAtk)
	{
		atkDt += dt;
	}

#if 0
	setRGBA(0, 0, 1, 1);
	setLineWidth(3);
	drawRect(pos.x - rtCol.size.width / 2 + off.x, pos.y - rtCol.size.height + off.y,
		rtCol.size.width, rtCol.size.height);
	setRGBA(1, 1, 1, 1);
#endif

	spr->paint(dt, pos + off, method, this, be, index, delta, leftRight, 3.0f * scale, true);
	//fbo->bind(texMinimap);
	//setRGBA(0, 1, 0, 1);
	//fillRect((hero->pos.x - hero->rtCol.size.width / 2) / 10.f, (hero->pos.y - hero->rtCol.size.height) / 10.0f,
	//	hero->rtCol.size.width / 10.0f, hero->rtCol.size.height / 10.0f);
	//setRGBA(1, 1, 1, 1);
	//fbo->unbind();



	if (be == HeroBehaveSlide)
	{
		slideDt += dt * 3.5f;
		if (slideDt > _slideDt)
			slideDt = _slideDt;
		float speedSlide = easeOut(slideDt / _slideDt, 300, 0);

		if (leftRight == true)
		{
			pos.x -= speedSlide * dt;
			if (pos.x < leftMax)
				pos.x = leftMax;
		}
		else
		{
			pos.x += speedSlide * dt;
			if (pos.x > rightMax)
				pos.x = rightMax;
		}
	}


	if (jumpCnt > 0)
	{
		// up
		pos.y += speedJump * dt;
		speedJump += speedFall * dt;
		if (speedJump >= 0.0f)
		{
			speedJump = 0.0f;
		}
	}

	if(dt != 0)
		key(dt, off);

#if 0
	setLineWidth(3);
	setRGBA(1, 0, 0, 1);
	iRect rtDefTmp = spr->getRtDef(be, index, pos + off, 3.0f * scale, leftRight);
	drawRect(rtDefTmp);
	iRect rtAttTmp = spr->getRtAtt(be, index, pos + off, 3.0f * scale, leftRight);
	drawRect(rtAttTmp);
	setRGBA(1, 1, 1, 1);
#endif
	bool hit = false;

	iRect rtAtt = spr->getRtAtt(be, index, pos + off, 3.0f * scale, leftRight);
	if (rtAtt.size.width == 0)
		return;
	//Enemy* e = NULL;
	for (int i = 0; i < entNum; i++)
	{
		Enemy* e = (Enemy*)entity[i];
		iRect enemyRtDef = e->spr->getRtDef(e->be, e->index, e->pos + off, 3.0f * e->scale, e->leftRight);
		if (containRect(rtAtt, enemyRtDef) && e->alive == true)
		{
			e->setDmg(ap);
		}
	}

	if (hit)
	{
		// 원래 사운드
		return;
	}
}



void Hero::key(float dt, iPoint off)
{
	if (be == HeroBehaveDie || be == HeroBehaveSlide || entNum == 0)
		return;

	iPoint v = iPointZero;

	if (keyStat & keysRight)
	{
		v.x = 1;
		leftRight = false;
	}
	else if (keyStat & keysLeft)
	{
		v.x = -1;
		leftRight = true;
	}

	if (v != iPointZero)
	{
		if (jumpCnt == 0 && be != HeroBehaveAttack1 && be != HeroBehaveAttack2 && be != HeroBehaveAttack3  &&be != HeroBehaveHurt)
		{
			if (pos.y == bottomMax)
				be = HeroBehaveRunField;
			else
				be = HeroBehaveFall;
		}
		v /= iPointLength(v);

		v *= (240.0f * speedMove * dt);

		int x = pos.x; x /= tileW;
		int y = pos.y; y /= tileH;

		if (v.x < 0)
		{
			float min = 0;
			int s;
			for (int i = x - 1; i > -1; i--)
			{
				s = pos.y; s /= tileH;
				if (tileMap[tileX * s + i].value > 0 && tileMap[tileX * s + i].ti.pass == false)
				{
					min = tileW * (i + 1);
					//exist = true;
					break;
				}
			}
			min += rtCol.size.width / 2;
			leftMax = min;
			pos.x += v.x;
			if (pos.x < min)
				pos.x = min;
		}
		else if (v.x > 0)
		{
			float max = tileW * maxTileX - 1;

			int s;
			for (int i = x + 1; i < maxTileX; i++)
			{
				s = pos.y; s /= tileH;
				if (tileMap[tileX * s + i].value > 0 && tileMap[tileX * s + i].ti.pass == false)
				{
					max = tileW * i - 1;
					break;
				}
			}
			max -= rtCol.size.width / 2;

			rightMax = max;
			pos.x += v.x;
			if (pos.x > max)
				pos.x = max;
		}
	}
	else
	{
		if (jumpCnt == 0)
		{
			if (be != HeroBehaveAttack1 && be != HeroBehaveHurt)
				be = (pos.y == bottomMax) ? HeroBehaveIdleField : HeroBehaveFall;
		}
	}

	if (keyDown & keysSpace)
	{
 		if (jumpCnt < 2)
		{
			jump();
			if (be != HeroBehaveAttack1 && be != HeroBehaveAttack2 && be != HeroBehaveAttack3)
			{
				be = HeroBehaveJump;
				index = 0;
				method = Hero::cbJump;
			}
		}
	}

	if (keyStat & keysA)
	{
#if 1
		if (be == HeroBehaveAttack1 || be == HeroBehaveAirattack1)
			return;
		be = (pos.y == bottomMax) ? HeroBehaveAttack1 : HeroBehaveAirattack1;
		index = 0;
		method = Hero::cbAttack;
#else
		printf("atkIndex = %d\n", atkIndex);
		printf("atkDt = %f\n", atkDt);
		if (isAtk == false)
		{
			isAtk = true;
			be = (pos.y == bottomMax) ? HeroBehaveAttack1 : HeroBehaveAirattack1;
			index = 0;
			atkIndex++;
			method = Hero::cbAttack;
		}
		else
		{
			if (atkDt < 0.2f || atkDt > 0.5f) return;
			else
			{
				printf("combo\n");
				switch (atkIndex)
				{
				case 1:
					printf("case 1\n");
					be = HeroBehaveAttack2;
					index = 0;
					atkDt = 0.0f;
					atkIndex++;
					method = Hero::cbAttack;
				case 2:
					printf("case 2\n");
					be = HeroBehaveAttack3;
					atkDt = 0.0f;
					index = 0;
					atkIndex++;
					method = Hero::cbAttack;
				}
			}
		}
#endif
	}

	if (keyDown & keysS)
	{
		if (cooldownSlide == _cooldownSlide)
		{
			cooldownSlide -= _cooldownSlide;

			if (be == HeroBehaveSlide || pos.y != bottomMax)
				return;
			be = HeroBehaveSlide;
			index = 0;

			method = Hero::cbSlide;
		}
	}
}

void Hero::setDmg(float dmg)
{
	Character::setDmg(dmg);
	if (hp > 0)
	{
		index = 0;
		be = HeroBehaveHurt;
		method = Hero::cbHurt;
	}
	else
	{
		index = 0;
		be = HeroBehaveDie;
		repeat = 0;
	}
}

void Hero::cbAttack(void* parm)
{
	Hero* h = (Hero*)parm;

	if (h->jumpCnt > 0)
		h->be = HeroBehaveFall;
	else
		h->be = HeroBehaveIdleField;
	h->index = 0;
	h->isAtk = false;
	h->atkDt = 0.0f;
	h->atkIndex = 0;
	h->method = NULL;
}

void Hero::cbJump(void* parm)
{
	Hero* h = (Hero*)parm;
	h->be = HeroBehaveFall;
	h->repeat = true;
}

void Hero::cbHurt(void* parm)
{
	Hero* h = (Hero*)parm;
	h->be = HeroBehaveIdleField;
	h->method = NULL;
}

void Hero::cbSlide(void* parm)
{
	Hero* h = (Hero*)parm;
	h->be = hero->pos.y == h->bottomMax ? HeroBehaveIdleField : HeroBehaveFall;
	h->method = NULL;
	slideDt = 0.0f;
}

// ---------------------------------
//   Enemy
// ---------------------------------

void loadEnemy()
{
}

void drawEnemy(float dt, iPoint off)
{
	// 생성된 enemy를 뿌리고 hp < 0 이 되면 pool로 반환
	for (int i = 0; i < entNum; i++)
	{
		Enemy* e = (Enemy*)entity[i];
		e->paint(dt, off);
		//fbo->bind(texMinimap);
		//setRGBA(1, 0, 0, 1);
		//fillRect((e->pos.x - e->rtCol.size.width / 2) / 10.f, (e->pos.y - e->rtCol.size.height) / 10.0f,
		//	e->rtCol.size.width / 10.0f, e->rtCol.size.height / 10.0f);
		//setRGBA(1, 1, 1, 1);
		//fbo->unbind();

		if (e->alive == false)
		{
			entNum--;
			entity[i] = entity[entNum];
			i--;
		}
	}

	for (int i = 0; i < projNum; i++)
	{
		projectile[i]->paint(dt, off);
		Projectile* proj = projectile[i];
		if (proj->hit == true)
		{
			projNum--;
			projectile[i] = projectile[projNum];
			i--;
		}
	}
}

void freeEnemy()
{
	// 필드에 생성된 Enemy를 전부 pool로 반환 ( 발생할 일 없음 )
}

void Enemy::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	Character::paint(dt, off);
}

void Enemy::setDmg(float dmg)
{
	if (be == EnemyBehaveDead)
		return;

	Character::setDmg(dmg);
	if (hp <= 0)
	{
		index = 0;
		be = EnemyBehaveDead;
		repeat = false;
		method = Enemy::cbDead;
	}
	else
	{
		if (kind == 3 && (be == EnemyBehaveAttack1 || be == EnemyBehaveAttack2))
			return;
		else
		{
			index = 0;
			be = EnemyBehaveHurt;
			repeat = false;
			method = Enemy::cbHurt;
		}

	}
}

bool shot = false;
void Enemy::cbAttack(void* parm)
{
	Enemy* e = (Enemy*)parm;
	//e->aiDt -= e->_aiDt;
	e->aiDt = 0.0f;
#if 1
	if (e->aiDt < 0.0f)
		e->aiDt = 0.0f;
#endif
	e->be = EnemyBehaveIdle;
	if (e->prevBe != e->be)
	{
		e->prevBe = e->be;
		e->index = 0;
	}
	e->method = NULL;
	shot = false;
}

void Enemy::cbDead(void* parm)
{
	Enemy* e = (Enemy*)parm;
	e->alive = false;
	//e->spr->method = NULL;
	e->be = EnemyBehaveIdle;
	e->method = NULL;
}

void Enemy::cbHurt(void* parm)
{
	Enemy* e = (Enemy*)parm;
	e->be = EnemyBehaveIdle;
	e->index = 0;
	e->method = NULL;
}

// ---------------------------------------
//   Walkable
// ---------------------------------------
Walkable::Walkable(int idx)
{
	loadSpriteChar(this, idx);
	// spr;
	kind = idx;

	leftRight = true;
	be = EnemyBehaveIdle;
	pos = iPointMake(-99999, -99999);
	alive = false;
	speedFall = 0;
	speedJump = 0;
	speedAttack = 2.0f;
	
	ap = 4;
	maxHp = 60;
	if (kind == 3)
		maxHp = 400;


	speedMove = 200.0f;
	if (kind == 0)
		speedMove = 220.0f;

	atkCol = iRectMake(0, 0, 90, 80);
	rangeDetect.size = iSizeMake(400, 300);
	rtCol.size = iSizeMake(30, 80);

	if (kind == 2)
	{
		atkCol = iRectMake(0, 0, 450, 80);
		speedMove = 170.0f;
		rangeDetect.size = iSizeMake(600, 200);
		ap = 3;
	}
	if(kind == 3)
	{
		maxHp = 600;
		rtCol.size = iSizeMake(50, 260);
		atkCol = iRectMake(0, 0, 400, 300);
		rangeDetect.size = iSizeMake(800, 300);
		ap = 15;
	}
	
	_randAidt = random() % 3 + 3;
	randAiDt = 0;

	rangeAttack.size = atkCol.size;

	includeAttack = false;
	_aiDt =
	aiDt = 2.0f;

	method = NULL;
	methodAI = methodAI0;
	if (kind == 3)
		methodAI = methodAI1;
}

Walkable::~Walkable()
{

}

void Walkable::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	Enemy::paint(dt, off);

#if _DEBUG
	setRGBA(0, 1, 1, 1);
	setLineWidth(3);
	drawRect(pos.x - rtCol.size.width / 2 + off.x, pos.y - rtCol.size.height + off.y,
		rtCol.size.width, rtCol.size.height);
	setRGBA(1, 1, 1, 1);
#endif

	spr->paint(dt, pos + off, method, this, be, index, delta, leftRight, 3.0f * scale, repeat);

	if (kind == 2 && be == EnemyBehaveAttack1 && index == 6 && shot == false)
	{
		genProjectile(this, leftRight, ap);
	}

	rangeDetect.origin = iPointMake(pos.x - rangeDetect.size.width / 2, pos.y - rangeDetect.size.height);
	rangeAttack.origin = iPointMake(pos.x - rangeAttack.size.width / 2, pos.y - rangeAttack.size.height);
	rtCol.origin = iPointMake(pos.x - rtCol.size.width / 2, pos.y - rtCol.size.height);

#if _DEBUG
	setRGBA(0, 0, 1, 1);
	drawRect(rangeDetect.origin.x + off.x, rangeDetect.origin.y + off.y,
			 rangeDetect.size.width, rangeDetect.size.height);
	setRGBA(1, 0, 0, 1);
	drawRect(rangeAttack.origin.x + off.x, rangeAttack.origin.y + off.y,
		rangeAttack.size.width, rangeAttack.size.height);
	setRGBA(1, 1, 1, 1);
#endif

	methodAI(this, dt);
#if 1
	iRect rtAtt = spr->getRtAtt(be, index, pos + off, 3.0f * scale, leftRight);
	if (rtAtt.size.width == 0)
		return;

	iRect rtDef = hero->spr->getRtDef(hero->be, hero->index, hero->pos + off, 3.0f * scale, hero->leftRight);
	if (rtDef.size.width == 0 || rtDef.size.height == 0) return;
	if (containRect(rtAtt, rtDef) && hero->hp > 0)
	{
		hero->setDmg(ap);
	}
#endif
}

void methodAI0(Enemy* e, float dt)
{
	if (hero->hp <= 0)
		return;
	// 맞고 있거나 때리고 있을 땐 동작 X;
	if (e->be == EnemyBehaveDead || e->be == EnemyBehaveHurt || e->be == EnemyBehaveAttack1 || e->be == EnemyBehaveAttack2)
		return;

	Sprite* spr = e->spr;

	iPoint v = iPointZero;
	v = hero->pos - e->pos;

	if (containPoint(hero->pos, e->rangeAttack))
	{
		printf("aiDt = %.f\n", e->aiDt);
		e->leftRight = !(v.x < 0);
		if (((Walkable*)e)->includeAttack == false)
		{
			((Walkable*)e)->includeAttack = true;
			e->aiDt = e->_aiDt;
		}

		e->aiDt += dt;
		if (e->aiDt < e->_aiDt)
		{
			e->be = EnemyBehaveIdle;
			if (e->prevBe != e->be)
			{
				e->prevBe = e->be;
				e->index = 0;
			}
		}
		else
		{
			e->be = EnemyBehaveAttack1;
			if (e->prevBe != e->be)
			{
				e->prevBe = e->be;
				e->index = 0;
			}
			//if (e->kind == 3)
			//{
			//	e->be = (random() % 2 == 0) ? EnemyBehaveAttack1 : EnemyBehaveAttack2;
			//}
			e->method = Enemy::cbAttack;
		}
	}
	// 탐색범위 안에 있으면,(공격범위 제외)
	else if (containPoint(hero->pos, e->rangeDetect))
	{
		e->leftRight = !(v.x < 0);
		if (fabsf(hero->pos.x - e->pos.x) > 2)
		{
			moveEntity(dt, e->pos, v.x > 0, e->speedMove, e->rtCol.size);
		}

		if (e->be != EnemyBehaveRun)
			e->be = EnemyBehaveRun;
#if 1
		if (fabsf(e->pos.x - hero->pos.x) > e->speedMove * e->_aiDt)
			((Walkable*)e)->includeAttack = false;
#endif
	}
	else
	{
		// 좌우로 랜덤하게  일정 시간마다 움직임
		if (e->randAiDt < e->_randAidt)
		{
			e->be = EnemyBehaveIdle;
			e->randAiDt += dt;
			if (e->randAiDt > e->_randAidt)
				e->randAiDt = e->_randAidt;
		}
		else if (e->randAiDt == e->_randAidt)
		{
			e->randomBe = random() % 3;
			if (e->randomBe == 1)
				e->be = EnemyBehaveRun;
			else if (e->randomBe == 2)
			{
				e->be = EnemyBehaveRun;
				e->leftRight = !e->leftRight;
			}
			e->randAiDt += 0.000001f;
		}
		else if (e->randAiDt < e->_randAidt + 1.0f)
		{
			e->randAiDt += dt;
			
			if (e->randomBe != 0)
			{
				moveEntity(dt, e->pos, e->leftRight, 100, e->rtCol.size);
			}

			if (e->randAiDt > e->_randAidt + 1.0f)
			{
				e->randAiDt -= e->_randAidt + 1.0f;
				e->_randAidt = random() % 3 + 3;
			}
		}
#if 1
		if (fabsf(e->pos.x - hero->pos.x) > e->speedMove * e->_aiDt)
			((Walkable*)e)->includeAttack = false;
#endif
	}
}

void methodAI1(Enemy* e, float dt)
{
	if (hero->hp <= 0)
		return;
	// 맞고 있거나 때리고 있을 땐 동작 X;
	if (e->be == EnemyBehaveDead || e->be == EnemyBehaveAttack1 || e->be == EnemyBehaveAttack2)
		return;

	Sprite* spr = e->spr;

	iPoint v = iPointZero;
	v = hero->pos - e->pos;

	if (containPoint(hero->pos, e->rangeAttack))
	{
		e->leftRight = !(v.x < 0);
		if (((Walkable*)e)->includeAttack == false)
		{
			((Walkable*)e)->includeAttack = true;
			e->aiDt = e->_aiDt;
		}

		e->aiDt += dt;
		if (e->aiDt < e->_aiDt)
		{
			if (e->be == EnemyBehaveHurt) return;
			e->be = EnemyBehaveIdle;
			if (e->prevBe != e->be)
			{
				e->prevBe = e->be;
				e->index = 0;
			}
		}
		else
		{
			e->be = EnemyBehaveAttack1;
			if (e->prevBe != e->be)
			{
				e->prevBe = e->be;
				e->index = 0;
			}
			if (e->kind == 3)
			{
				e->be = (random() % 2 == 0) ? EnemyBehaveAttack1 : EnemyBehaveAttack2;
			}
			e->method = Enemy::cbAttack;
		}
	}
	// 탐색범위 안에 있으면,(공격범위 제외)
	else if (containPoint(hero->pos, e->rangeDetect))
	{
		if (e->be == EnemyBehaveHurt) return;
		e->leftRight = !(v.x < 0);
		if (fabsf(hero->pos.x - e->pos.x) > 2)
		{
			moveEntity(dt, e->pos, v.x > 0, e->speedMove, e->rtCol.size);
		}

		if (e->be != EnemyBehaveRun)
			e->be = EnemyBehaveRun;
#if 1
		if (fabsf(e->pos.x - hero->pos.x) > e->speedMove * e->_aiDt)
			((Walkable*)e)->includeAttack = false;
#endif
	}
	else
	{
		// 좌우로 랜덤하게  일정 시간마다 움직임
		if (e->randAiDt < e->_randAidt)
		{
			e->be = EnemyBehaveIdle;
			e->randAiDt += dt;
			if (e->randAiDt > e->_randAidt)
				e->randAiDt = e->_randAidt;
		}
		else if (e->randAiDt == e->_randAidt)
		{
			e->randomBe = random() % 3;
			if (e->randomBe == 1)
				e->be = EnemyBehaveRun;
			else if (e->randomBe == 2)
			{
				e->be = EnemyBehaveRun;
				e->leftRight = !e->leftRight;
			}
			e->randAiDt += 0.000001f;
		}
		else if (e->randAiDt < e->_randAidt + 1.0f)
		{
			e->randAiDt += dt;

			if (e->randomBe != 0)
			{
				moveEntity(dt, e->pos, e->leftRight, 100, e->rtCol.size);
			}

			if (e->randAiDt > e->_randAidt + 1.0f)
			{
				e->randAiDt -= e->_randAidt + 1.0f;
				e->_randAidt = random() % 3 + 3;
			}
		}
#if 1
		if (fabsf(e->pos.x - hero->pos.x) > e->speedMove * e->_aiDt)
			((Walkable*)e)->includeAttack = false;
#endif
	}

}

void methodAI2(Enemy* e, float dt)
{

}
void methodAI3(Enemy* e, float dt)
{

}

void genProjectile(Enemy* e, bool dir, int ap)
{
	//
	for (int i = 0; i < 50; i++)
	{
		Projectile* proj = projPool[0][i];

		if (proj->gen == false)
		{
			proj->gen = true;
			proj->pos = e->pos;
			proj->pos.y -= 30;
			proj->leftRight = dir;
			proj->ap = ap;

			projectile[projNum] = proj;
			projNum++;
			shot = true;
			break;
		}
	}
}

