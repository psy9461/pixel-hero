#include "Sprite.h"

#include "iStd.h"

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
			fwrite(&f->delta, 1, sizeof(float), pf);
			fwrite(&f->rtAtt, 1, sizeof(iRect), pf);
			fwrite(&f->rtDef, 1, sizeof(iRect), pf);
		}
	}

	fclose(pf);
}

Tex* _tex = NULL;
int texNum;
#define _texNum 100

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
	texNum++;

	int len = strlen(path);
	x->path = new char[len + 1];
	strcpy(x->path, path);
#if 1
	setTexture(CLAMP, NEAREST);
#endif
	x->tex = createImage(path);

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
}

Sprite::Sprite(const char* szForamt, ...)
{
	char szText[1024];
	va_start_end(szText, szForamt);
	FILE* pf = fopen(szText, "rb");

	int num;
	fread(&num, 1, sizeof(int), pf);

	arrayBe = new iArray(cbBe);
	for (int i = 0; i < num; i++)
	{
		int n;
		fread(&n, 1, sizeof(int), pf);

		iArray* arrayFrame = new iArray(cbFrame);
		for (int j = 0; j < n; j++)
		{
			Frame* f = new Frame;

			int len;
			fread(&len, 1, sizeof(int), pf);
			f->t->path = new char[len + 1];
			fread(f->t->path, len, sizeof(char), pf);
			f->t->path[len] = 0;
			f->t->tex = addTextureInTex(f->t->path);// createImage(f->t->path);
			fread(&f->p, 1, sizeof(iPoint), pf);
			fread(&f->delta, 1, sizeof(float), pf);
			fread(&f->delta, 1, sizeof(float), pf);
			fread(&f->rtAtt, 1, sizeof(iRect), pf);
			fread(&f->rtDef, 1, sizeof(iRect), pf);

			arrayFrame->addObject(f);
		}

		arrayBe->addObject(arrayFrame);
	}

	fclose(pf);

	index = 0;
	delta = 0.0f;

	scale = 1.0f;
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
	if(f->t->tex)
		freeImage(f->t->tex);
	delete f->t->path;
	delete f->t;
	delete f;
}

void Sprite::paint(float dt, iPoint off)
{
	iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(be);
	Frame* f = (Frame*)arrayFrame->objectAtIndex(index);
	delta += dt;
	if (delta >= f->delta)
	{
		delta -= f->delta;
		index++;
		if (index == arrayFrame->count)
			index = 0;// repeat
	}

	Texture* tex = f->t->tex;
	iPoint p = position + f->p * scale + off;
	drawImage(tex, p.x, p.y, TOP | LEFT, 0, 0, tex->width, tex->height,
		scale, scale, 2, 0, REVERSE_NONE);
}

iRect Sprite::getRtAtt(bool reverse)
{
	iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(be);
	Frame* f = (Frame*)arrayFrame->objectAtIndex(index);
	//if (reverse)
	//{
	//	return iRectMake()
	//}

	return f->rtAtt;
}

iRect Sprite::getRtDef(bool reverse)
{
	iArray* arrayFrame = (iArray*)arrayBe->objectAtIndex(be);
	Frame* f = (Frame*)arrayFrame->objectAtIndex(index);
	//if (reverse)
	//{
	//	return 
	//}

	return f->rtDef;
}

