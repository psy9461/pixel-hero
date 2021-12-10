#include "iStrTex.h"

#include "iStd.h"

iStrTex::iStrTex(MethodStrTex method)
{	// str = "";
	str = new char[1];
	str[0] = 0;

	this->method = method;
	tex = NULL;

	widthMax = 1024.0f;
}

iStrTex::~iStrTex()
{
	delete str;

	if (tex)
		freeImage(tex);
}

void iStrTex::setString(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szText, szFormat);

	if (strcmp(szText, str) == 0)
		return;

	delete str;
	str = new char[strlen(szText) + 1];
	strcpy(str, szText);

	//if (tex)
	//	freeImage(tex);
	if (tex)
		glDeleteTextures(1, &tex->texID);
	else
	{
		tex = new Texture;
		tex->retainCount = 1;
	}

	if (method)
	{
		float ss = getStringSize();
		float fr, fg, fb, fa;
		getStringRGBA(fr, fg, fb, fa);
		float sb = getStringBorder();
		float br, bg, bb, ba;
		getStringBorderRGBA(br, bg, bb, ba);

		Texture* t = method(szText);
		tex->texID = t->texID;
		tex->width = t->width;
		tex->height = t->height;
		tex->potWidth = t->potWidth;
		tex->potHeight = t->potHeight;
		//tex->retainCount = t->retainCount;
		delete t;

		setStringSize(ss);
		setStringRGBA(fr, fg, fb, fa);
		setStringBorder(sb);
		setStringBorderRGBA(br, bg, bb, ba);
	}
	else
	{
		iGraphics* g = iGraphics::share();
#if 0 
		iRect rt = iGraphics::rectOfString(szText);
		g->init(rt.size);

		g->drawString(0, 0, TOP | LEFT, str);
#else
		int lineNum;
		char** line = getStringLineWidth(szText, lineNum, widthMax);

		float width = 0;
		float height = 0;
		iRect rt;
		for (int i = 0; i < lineNum; i++)
		{
			rt = iGraphics::rectOfString(line[i]);
			if (width < rt.size.width)
				width = rt.size.width;
			if (height < rt.size.height)
				height < rt.size.height;
		}
		float slh = getStringLineHeight();
		if (slh < height)//getStringSize())
			slh = height;//getStringSize();
		height = slh * lineNum;
		g->init(width, height);

		//float br, bg, bb, ba;
		//getRGBA(br, bg, bb, ba);
		//setRGBA(0, 1, 0, 1);
		//g->fillRect(0, 0, width, height);
		for (int i = 0; i < lineNum; i++)
		{
			rt = iGraphics::rectOfString(line[i]);
			g->drawString(0, slh * i, TOP | LEFT, line[i]);
		}
		//setRGBA(br, bg, bb, ba);

		iString::freeStringLine(line, lineNum);
#endif
		//tex = g->getTexture();
		Texture* t = g->getTexture();
		tex->texID = t->texID;
		tex->width = t->width;
		tex->height = t->height;
		tex->potWidth = t->potWidth;
		tex->potHeight = t->potHeight;
		//tex->retainCount = t->retainCount;
		delete t;
	}
}

char** iStrTex::getStringLineWidth(const char* str, int& lineNum, float width, int d)
{
	char** res = new char* [100];
	int resNum = 0;

	// str = "Good afternoon. I am Korean.";
	int off = 0;
	int b = 0;
	for (int i = 0; str[i]; i += b)
	{
		if ((str[i] & 128) == 0) b = 1;			// ?XXX XXXX
		else if ((str[i] & 224) == 192) b = 2;	// 110X XXXX 128 + 64
		else if ((str[i] & 240) == 224) b = 3;	// 1110 XXXX 128 + 64 + 32
		
		int n = i + b - off;
		char result[128];
		memcpy(result, &str[off], n);
		result[n] = 0;

		if (str[i] == d)
		{
			result[n - 1] = 0;//result[i] = 0;
			b++;//i++;
			off = i + 1;

			char* r = new char[n];// n-1 +1
			strcpy(r, result);
			res[resNum] = r;
			resNum++;
		}
		else if (iGraphics::rectOfString(result).size.width > width)
		{
			result[n - b] = 0;//result[i] = 0;
			off = i;
			i -= b;

			char* r = new char[n - b + 1];
			strcpy(r, result);
			res[resNum] = r;
			resNum++;
		}
	}
	int n = strlen(str);
	if (off < n)
	{
		char* r = new char[n - off + 1];
		strcpy(r, &str[off]);
		res[resNum] = r;
		resNum++;
	}

	lineNum = resNum;
	return res;
}

void iStrTex::paint(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szText, szFormat);

	setString(szText);
	drawImage(tex, x, y, anc, 0, 0, tex->width, tex->height, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
}

void iStrTex::paint(iPoint p, int anc, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szText, szFormat);

	setString(szText);
	drawImage(tex, p.x, p.y, anc, 0, 0, tex->width, tex->height, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
}

void iStrTex::paint(float x, float y, int anc)
{
	drawImage(tex, x, y, anc, 0, 0, tex->width, tex->height, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
}

void iStrTex::paint(iPoint p, int anc)
{
	drawImage(tex, p.x, p.y, anc, 0, 0, tex->width, tex->height, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
}
