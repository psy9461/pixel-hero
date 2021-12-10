#include "iShaderToy.h"

#include "iStd.h"

iShaderToy::iShaderToy(STInfo* info)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float position[16] = {
		-1, 1,0,1,		1, 1,0,1,
		-1,-1,0,1,		1,-1,0,1
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, position, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbe);
	GLubyte indices[6] = {0, 1, 2,  1, 2, 3};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	programID = new GLuint[5];
	texFbo = new Texture**[5];
	bind = new STBind[5];

	int length;
	char* str = loadFile(info->strVert, length);
	GLuint vertID = createShaderID(str, 0);
	delete str;
	int i, j;
	for (i = 0; i < 5; i++)
	{
		STFrag* frag = &info->frag[i];
		if (frag->strFrag == NULL)
		{
			programID[i] = 0;
			texFbo[i] = new Texture*[2];
			for (j = 0; j < 2; j++)
				texFbo[i][j] = NULL;
			continue;
		}

#if 0
		GLuint fragID = createShaderID(frag->strFrag, 1);
#else
		char path[256];
		sprintf(path, "%s_", frag->strFrag);

		int length;
		char* strBody = loadFile(frag->strFrag, length);

		char* str = new char[length + 1024];
		const char* strFirst =
			"#version 150							\n\
			#ifdef GL_ES							\n\
						precision mediump float;	\n\
			#endif									\n\
													\n\
			uniform vec3      iResolution;          \n\
			uniform float     iTime;                \n\
			uniform float     iTimeDelta;           \n\
			uniform int       iFrame;               \n\
			uniform float     iChannelTime[4];      \n\
			uniform vec3      iChannelResolution[4];\n\
			uniform vec4      iMouse;               \n\
			uniform int       iKeyboard[5];         \n\
			uniform sampler2D iChannel0;            \n\
			uniform sampler2D iChannel1;            \n\
			uniform sampler2D iChannel2;            \n\
			uniform sampler2D iChannel3;            \n\
			uniform vec4      iDate;                \n\
			uniform float     iSampleRate;          \n\
			out vec4 fragColor; \n";
		strcpy(str, strFirst);
		strcpy(&str[strlen(str)], strBody); //strcat(str, strBody);
		delete strBody;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		const char* strEnd = 
			"void main() {								\n\
				mainImage(fragColor, gl_FragCoord.xy);	\n\
			}";
		//strcat(str, strEnd);
		strcpy(&str[strlen(str)], strEnd);

		GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragID, 1, &str, NULL);
		glCompileShader(fragID);
		checkShaderID(fragID);
#endif
		programID[i] = createProgramID(vertID, fragID);
		freeShaderID(fragID);

		texFbo[i] = new Texture*[2];
		if (i < 4)
		{
			for (j = 0; j < 2; j++)
				texFbo[i][j] = createTexture(devSize.width, devSize.height, true);
		}

		for (j = 0; j < 4; j++)
		{
			bind[i].texs[j] = frag->texs[j];
			bind[i].bufs[j] = frag->bufs[j];
			bind[i].filter[j] = frag->filter[j];
			bind[i].wrap[j] = frag->wrap[j];
		}
	}
	freeShaderID(vertID);

	takeTime = 100.0f;
	mouse = new float[4];
	onMouse = false;
	keyboard = new int[5];
	memset(mouse, 0x00, sizeof(float) * 4);
	frame = 0;

	toggle = false;
}
iShaderToy::~iShaderToy()
{
	int i, j;

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbe);

	for (i = 0; i < 5; i++)
	{
		if( programID[i] )
			freeProgramID(programID[i]);
	}
	delete programID;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 2; j++)
		{
			if (texFbo[i][j])
				freeImage(texFbo[i][j]);
		}
		delete texFbo[i];
	}
	delete texFbo;
	delete bind;

	delete mouse;
	delete keyboard;
}

void iShaderToy::paint(float dt, iPoint off)
{
	//glDisable(GL_BLEND);//////////////////////////////////////////
	TextureWrap wrap;
	TextureFilter minFilter, magFilter;
	getTexture(wrap, minFilter, magFilter);

	for (int i = 0; i < 5; i++)
	{
		if (i < 4)
		{
			Texture* tex = texFbo[i][toggle];
			if (tex == NULL)
				continue;

			//setTexture(tex->texID, wrap, minFilter, magFilter);
			fbo->bind(tex);
		}
		//glClearColor(1, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint id = programID[i];
		glUseProgram(id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		GLuint attr = glGetAttribLocation(id, "position");
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);
		GLuint attrPosition = attr;

		GLuint loc = glGetUniformLocation(id, "iResolution");
		glUniform3f(loc, devSize.width, devSize.height, 0.0f);
		loc = glGetUniformLocation(id, "iTime");
		glUniform1f(loc, takeTime);
		loc = glGetUniformLocation(id, "iTimeDelta");
		glUniform1f(loc, dt);
		loc = glGetUniformLocation(id, "iFrame");
		glUniform1i(loc, frame);
		loc = glGetUniformLocation(id, "iMouse");
		glUniform4f(loc, mouse[0], mouse[1], mouse[2], mouse[3]);
		//glUniform4fv(loc, 1, mouse);
		loc = glGetUniformLocation(id, "iKeyboard");
		glUniform1iv(loc, 5, keyboard);

		STBind* b = &bind[i];
		for (int j = 0; j < 4; j++)
		{
			Texture* tex = b->texs[j];
			if (tex == NULL && b->bufs[j] != -1)
				tex = texFbo[b->bufs[j]][!toggle];
			if (tex == NULL) continue;

			char s[32];
			sprintf(s, "iChannel%d", j);
			loc = glGetUniformLocation(id, s);
			glUniform1i(loc, j);

			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, tex->texID);
			setTexture(b->wrap[j], b->filter[j]);

			sprintf(s, "iChannelResolution[%d]", j);
			loc = glGetUniformLocation(id, s);
			glUniform3f(loc, tex->width, tex->height, 0.0f);
		}

		// 런
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(attrPosition);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (i < 4)
			fbo->unbind();
	}
	//glEnable(GL_BLEND);//////////////////////////////////////////

	takeTime += dt;
	frame++;

	toggle = !toggle;

	keyboard[0] = (keyDown & keysLeft);
	keyboard[1] = (keyDown & keysRight);
	keyboard[2] = (keyDown & keysUp);
	keyboard[3] = (keyDown & keysDown);
	keyboard[4] = (keyDown & keysSpace);
}
void iShaderToy::key(iKeyStat stat, iPoint point)
{
	// mouse.xy : 현재 마우스 위치
	// mouse.zw : 처음 마우스 누른 위치
	switch (stat) {

	case iKeyStatBegan:
		mouse[0] = point.x;
		mouse[1] = devSize.height - point.y;
		mouse[2] = point.x;
		mouse[3] = devSize.height - point.y;
		onMouse = true;
		break;

	case iKeyStatMoved:
		if (onMouse)
		{
			mouse[0] = point.x;
			mouse[1] = devSize.height - point.y;
		}
		break;

	case iKeyStatEnded:
		mouse[2] *= -1;
		mouse[3] *= -1;
		onMouse = false;
		break;
	}
}
