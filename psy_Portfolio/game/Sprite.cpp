#include "Sprite.h"


void Sprite::save(const char* fileName)
{
	FILE* pf = fopen(fileName, "wb");

	fwrite(&arrayBe->count, 1, sizeof(int), pf);
	for (int i = 0; i < arrayBe->count; i++)
	{
		iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(i);
		fwrite(&arrayFrame->count, 1, sizeof(int), pf);
		for (int j = 0; j < arrayFrame->count; j++)
		{
			Frame* f = (Frame*)arrayFrame->objectAtIndex(j);
			int len = strlen(f->t->path);
			fwrite(&len, 1, sizeof(int), pf);
			fwrite(f->t->path, len, sizeof(char), pf);
			fwrite(&f->p, 1, sizeof(iPoint), pf);
			fwrite(&f->delta, 1, sizeof(float), pf);
			fwrite(&f->rtAtt, 1, sizeof(iRect), pf);
			fwrite(&f->rtDef, 1, sizeof(iRect), pf);
		}
	}

	fclose(pf);
}

Tex* _tex = NULL;
int texNum;
#define _texNum 1000

Texture* addTextureInTex(const char* path)
{
	if (_tex == NULL)
	{
		_tex = new Tex[_texNum];
		texNum = 0;
	}
	for (int i = 0; i < texNum; i++)
	{
		if (strcmp(path, _tex[i].path) == 0)
		{
			_tex[i].tex->retainCount++;
			return _tex[i].tex;
		}
	}

	Tex* x = &_tex[texNum];
#if 1
	if (texNum >= _texNum)
		printf("fatal error\n");
#endif
	texNum++;

	int len = strlen(path);
	x->path = new char[len + 1];
	strcpy(x->path, path);
	setTexture(CLAMP, NEAREST);
	x->tex = createImage(path);
	setTexture(CLAMP, LINEAR);

	return x->tex;
}

bool delTextureInTex(const char* path)
{
	if (_tex == NULL)
		return false;

	for (int i = 0; i < texNum; i++)
	{
		if (strcmp(path, _tex[i].path) == 0)
		{
			freeImage(_tex[i].tex);
			delete _tex[i].path;

			texNum--;
			memcpy(&_tex[i], &_tex[texNum], sizeof(Tex));
			return true;
		}
	}
	return false;
}

void delAllTextureInTex()
{
	for (int i = 0; i < texNum; i++)
	{
		delete _tex[i].path;
		_tex[i].tex->retainCount = 1;
		freeImage(_tex[i].tex);
	}
}

Sprite::Sprite()
{
	arrayBe = new iArray(cbBe);
#if 0
	prevBe = -1
	be = 0;
	index = 0;
	delta = 0.0f;
#endif
}

Sprite::Sprite(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szText, szFormat);

	int len;
	char* buf = loadFile(szText, len);
	len = 0;

	arrayBe = new iArray(cbBe);

	int beNum;
	memcpy(&beNum, &buf[len], sizeof(int)); len += sizeof(int);
	for (int i = 0; i < beNum; i++)
	{
		int lenState;
		memcpy(&lenState, &buf[len], sizeof(int)); len += sizeof(int);
		len += sizeof(char) * lenState;

		//iArray* arrayFrame = new iArray(Sprite::cbFrame);
		iArray* arrayFrame = new iArray(cbFrame);
		arrayBe->addObject(arrayFrame);

		int frameNum;
		memcpy(&frameNum, &buf[len], sizeof(int)); len += sizeof(int);
		for (int j = 0; j < frameNum; j++)
		{
			Frame* f = new Frame;// (Frame*)arrayFrame->objectAtIndex(j);

			int lenPath;
			memcpy(&lenPath, &buf[len], sizeof(int)); len += sizeof(int);
			char* path = new char[lenPath + 1];
			memcpy(path, &buf[len], sizeof(char) * lenPath); len += sizeof(char) * lenPath;
			path[lenPath] = 0;
			f->t = new Tex;
			f->t->path = path;
			f->t->tex = addTextureInTex(path);
			memcpy(&f->p, &buf[len], sizeof(iPoint)); len += sizeof(iPoint);
			memcpy(&f->delta, &buf[len], sizeof(float)); len += sizeof(float);
			memcpy(&f->rtAtt, &buf[len], sizeof(iRect)); len += sizeof(iRect);
			memcpy(&f->rtDef, &buf[len], sizeof(iRect)); len += sizeof(iRect);

			arrayFrame->addObject(f);
		}
	}
	
	delete buf;

#if 0
	prevBe = -1;
	be = 0;
	index = 0;
	delta = 0.0f;
#endif
}

Sprite::~Sprite()
{
	delete arrayBe;
}

void Sprite::cbBe(void* parm)
{
	iArray* a = (iArray*)parm;
	delete a;
}
void Sprite::cbFrame(void* parm)
{
	Frame* f = (Frame*)parm;
	freeImage(f->t->tex);
	delete f->t->path;
	delete f->t;
	delete f;
}

void Sprite::paint(float dt, iPoint pos, MethodAfter m, void* parm, int be, int& index, float& delta, bool leftRight, float scale, bool repeat)
{
	iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(be);
	if (index >= arrayFrame->count)
		index = 0;
	Frame* f = (Frame*)arrayFrame->objectAtIndex(index);

	if (f == NULL) return;
	
	delta += dt;
	if (delta >= f->delta)
	{
		delta -= f->delta;
		index++;
		if (index >= arrayFrame->count)
		{
			if (repeat)
				index = 0;// repeat
			if (m)
				m(parm);
		}
	}

	Texture* tex = f->t->tex;
	iPoint t = f->p;
	if (leftRight)
		t.x = -(tex->width - fabsf(t.x));
	t *= scale;
	iPoint p = pos + t;

	drawImage(tex, p.x, p.y, TOP | LEFT, 0, 0, tex->width, tex->height,
		scale, scale, 2, 0, leftRight ? REVERSE_WIDTH : REVERSE_NONE);
}


iRect Sprite::getRtAtt(int be, int index, iPoint pos, float scale, bool reverse)
{
	iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(be);
	Frame* f = (Frame*)arrayFrame->objectAtIndex(index);

	if (f == NULL) return iRectZero;
	iRect frt = f->rtAtt;
	
	iRect rtAtt = iRectMake(pos.x + frt.origin.x * scale, pos.y + frt.origin.y * scale,
							frt.size.width * scale, frt.size.height * scale);
	if (reverse)
	{
		rtAtt = iRectMake(pos.x - (frt.origin.x + frt.size.width) * scale, pos.y + frt.origin.y * scale,
			frt.size.width * scale, frt.size.height * scale);
	}

	return rtAtt;
}

iRect Sprite::getRtDef(int be, int index, iPoint pos, float scale, bool reverse)
{
	iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(be);
	Frame* f = (Frame*)arrayFrame->objectAtIndex(index);
	
	if (f == NULL) return iRectZero;
	iRect frt = f->rtDef;

	iRect rtDef = iRectMake(pos.x + f->rtDef.origin.x * scale, pos.y + f->rtDef.origin.y * scale,
							f->rtDef.size.width * scale, f->rtDef.size.height * scale);
	if (reverse)
	{
		rtDef = iRectMake(pos.x - (frt.origin.x + frt.size.width) * scale, pos.y + frt.origin.y * scale,
						  frt.size.width * scale, frt.size.height * scale);
	}

	return rtDef;
}
