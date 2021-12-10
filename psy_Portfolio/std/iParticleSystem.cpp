#include "iParticleSystem.h"

#include "iStd.h"

iVBO* vbo;
Texture* texEgg;

iVBO::iVBO()
{
	quads = NULL;
	quadsMax = 0;
	quadsNum = 0;

	quadsID = 0;
	elementsID = 0;

	tex = NULL;
	position = iPointZero;

	blend = GLBlendLight;
}

iVBO::~iVBO()
{
	clean();
}

void iVBO::clean()
{
	if (tex == NULL)
		return;

	freeImage(tex);
	tex = NULL;

	delete quads;
	glDeleteBuffers(1, &quadsID);
	glDeleteBuffers(1, &elementsID);
}

void iVBO::set(Texture* tex, int max)
{
	clean();

	tex->retainCount++;
	this->tex = tex;

	quads = new Quad[max];
	quadsMax = max;
	quadsNum = 0;

	GLushort* indices = new GLushort[max * 6];
	for (int i = 0; i < max; i++)
	{
		quads[i].tl.st = iPointMake(0, 0);
		quads[i].tr.st = iPointMake(1, 0);
		quads[i].bl.st = iPointMake(0, 1);
		quads[i].br.st = iPointMake(1, 1);

		indices[6 * i + 0] = 4 * i + 0;
		indices[6 * i + 1] = 4 * i + 1;
		indices[6 * i + 2] = 4 * i + 2;

		indices[6 * i + 3] = 4 * i + 1;
		indices[6 * i + 4] = 4 * i + 2;
		indices[6 * i + 5] = 4 * i + 3;
	}

	glGenBuffers(1, &quadsID);
	glBindBuffer(GL_ARRAY_BUFFER, quadsID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad) * max, NULL, GL_DYNAMIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Quad) * max, quads, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &elementsID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * max, indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete indices;
}

void iVBO::paint()
{
	glBindBuffer(GL_ARRAY_BUFFER, quadsID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Quad) * quadsNum, quads);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->texID);

	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (const void*)offsetof(Vertex, coord));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (const void*)offsetof(Vertex, color));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (const void*)offsetof(Vertex, st));

	GLBlend b = getGLBlend();
	setGLBlend(blend);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsID);
	glDrawElements(GL_TRIANGLES, 6 * quadsNum, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	setGLBlend(b);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#define use_vbo 1
bool Egg::paint(float dt)
{
	float r = life / _life;
	p += v * dt;
	float s = linear(r, sStart, sEnd);
#if (use_vbo==0)
	setRGBA(linear(r, cStart.r, cEnd.r),
			linear(r, cStart.g, cEnd.g),
			linear(r, cStart.b, cEnd.b),
			linear(r, cStart.a, cEnd.a));
	drawImage(texEgg, p.x, p.y, VCENTER | HCENTER,
		0, 0, texEgg->width, texEgg->height,
		s, s, 2, 0);
#else
	Quad* q = &vbo->quads[vbo->quadsNum];
	vbo->quadsNum++;

	float hw = texEgg->width * s / 2;
	float hh = texEgg->height * s / 2;

	q->tl.coord.x = p.x - hw;
	q->tl.coord.y = p.y - hh;
	q->tl.color.r = linear(r, cStart.r, cEnd.r);
	q->tl.color.g = linear(r, cStart.g, cEnd.g);
	q->tl.color.b = linear(r, cStart.b, cEnd.b);
	q->tl.color.a = linear(r, cStart.a, cEnd.a);
	//q->tl.st.x = 0;
	//q->tl.st.y = 0;

	q->tr.coord.x = p.x + hw;
	q->tr.coord.y = p.y - hh;
	q->tr.color = q->tl.color;
	//q->tr.st.x = 1;
	//q->tr.st.y = 0;

	q->bl.coord.x = p.x - hw;
	q->bl.coord.y = p.y + hh;
	q->bl.color = q->tl.color;
	//q->tr.st.x = 0;
	//q->tr.st.y = 1;

	q->br.coord.x = p.x + hw;
	q->br.coord.y = p.y + hh;
	q->br.color = q->tl.color;
	//q->br.st.x = 1;
	//q->br.st.y = 1;
#endif

	life += dt;
	if (life < _life)
		return false;
	return true;
}

iParticleSystem::iParticleSystem(int quadMax)
{
	_egg = new Egg[quadMax];
	egg = new Egg*[quadMax];
	eggNum = 0;

//	_createDt = 0.002f;
	_createDt = 0.0001f;
	createDt = 0.0f;
	life = 0.5f; lifeVar = 0.1f;// 0.4 ~ 0.6

	position = iPointMake(devSize.width / 2, devSize.height / 2);
	positionVar = iPointMake(5, 5);
	move = 200; moveVar = 20;// 180 ~ 220 px
	degree = 90; degreeVar = 10;
	cStart = iColor4bMake(200, 20, 20, 200);
	cStartVar = iColor4bMake(55, 10, 10, 55);
	cEnd = iColor4bMake(20, 20, 200, 55);
	cEndVar = iColor4bMake(10, 10, 55, 55);
	sStart = 0.3f; sStartVar = 0.1f;// 0.1 ~ 0.3f
	sEnd = 0.5f; sEndVar = 0.2f;// 0.1 ~ 0.3f

	texEgg = createParticle();;
	vbo = new iVBO();
	vbo->set(texEgg, quadMax);
	freeImage(texEgg);
}

iParticleSystem::~iParticleSystem()
{
	delete _egg;
	delete egg;
	delete vbo;
}

float randomM1to1()
{
	float n = (random() % 201) - 100;
	return n / 100.0f;
}

void iParticleSystem::paint(float dt)
{
	iFBO::clear(0, 0, 0, 1);
	static Texture* t = createImage("assets/map/mapBg.jpg");
	drawImage(t, devSize.width / 2, devSize.height / 2, VCENTER | HCENTER,
	0, 0, t->width, t->height, 2.0f, 2.0f, 2, 0);

	int i;

	vbo->quadsNum = 0;
#if (use_vbo==0)
	setGLBlend(GLBlendLight);
#endif
	for (i = 0; i < eggNum; i++)
	{
		if (egg[i]->paint(dt))
		{
			eggNum--;
			egg[i] = egg[eggNum];
			i--;
		}
	}
#if use_vbo
	vbo->paint();
#endif
	setGLBlend(GLBlendAlpha);

	createDt += dt;
	for (; createDt > _createDt; createDt -= _createDt)
	{
		Egg* e = NULL;
		for (i = 0; i < _quadMax; i++)
		{
			e = &_egg[i];
			if (e->life >= e->_life)
				break;
		}
		// settings
		e->_life = life + lifeVar * randomM1to1();
		e->life = 0.0f;
		e->p.x = position.x + positionVar.x * randomM1to1();
		e->p.y = position.y + positionVar.y * randomM1to1();
		e->v = 
		iPointRotate(iPointMake(1, 0), iPointZero, degree + degreeVar * randomM1to1())
			* (move + moveVar * randomM1to1());
		e->cStart.r = cStart.r + cStartVar.r * randomM1to1(); e->cStart.r /= 255.0f;
		e->cStart.g = cStart.g + cStartVar.g * randomM1to1(); e->cStart.g /= 255.0f;
		e->cStart.b = cStart.b + cStartVar.b * randomM1to1(); e->cStart.b /= 255.0f;
		e->cStart.a = cStart.a + cStartVar.a * randomM1to1(); e->cStart.a /= 255.0f;
		e->cEnd.r = cEnd.r + cEndVar.r * randomM1to1(); e->cEnd.r /= 255.0f;
		e->cEnd.g = cEnd.g + cEndVar.g * randomM1to1(); e->cEnd.g /= 255.0f;
		e->cEnd.b = cEnd.b + cEndVar.b * randomM1to1(); e->cEnd.b /= 255.0f;
		e->cEnd.a = cEnd.a + cEndVar.a * randomM1to1(); e->cEnd.a /= 255.0f;
		e->sStart = sStart + sStartVar * randomM1to1();
		e->sEnd = sEnd + sEndVar * randomM1to1();

		// add
		egg[eggNum] = e;
		eggNum++;
	}
}

Texture* createParticle()
{
	float ox = 16;
	float oy = 16;

	uint8* rgba = new uint8[32 * 32 * 4];
	for (int j = 0; j < 32; j++)
	{
		for (int i = 0; i < 32; i++)
		{
			uint8* c = &rgba[32 * 4 * j + 4 * i];
			memset(c, 0xFF, sizeof(uint8) * 3);

			float x = i + 0.5f;
			float y = j + 0.5f;
			// (x,y) (ox,oy)
			float d = sqrtf((x - ox)*(x - ox) + (y - oy)*(y - oy));
			if (d < 15.5f)
				c[3] = (1.0f - d / 15.5f) * 0xFF;
			else
				c[3] = 0;
		}
	}

	Texture* tex = createImageWithRGBA(rgba, 32, 32);
	delete rgba;

	return tex;
}

