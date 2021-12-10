#include "iOpenGL.h"

#include "iStd.h"

HGLRC hRC;

iMatrix *matrixProj, *matrixView;
Texture* texStd;

void loadOpenGL(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0x00, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;

    int pixelformat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelformat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    printf("Status: Using GL %s, GLEW %s\n",
        glGetString(GL_VERSION), glewGetString(GLEW_VERSION));
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Error: %s\n", glewGetErrorString(err));
        return;
    }
#if 1// #add
    if (wglewIsSupported("WGL_ARB_create_context"))
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);

        int attr[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 2,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0
        };
        hRC = wglCreateContextAttribsARB(hDC, NULL, attr);
        wglMakeCurrent(hDC, hRC);
    }
    //const char* strGL = (const char*)glGetString(GL_VERSION);
    //const char* strGLEW = (const char*)glewGetString(GLEW_VERSION);
    //const char* strGLSL = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    //printf("gl(%s) glew(%s) glsl(%s)\n", strGL, strGLEW, strGLSL);
#endif// #add

    // ---------------------------------------------------------

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#if 0// for android
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#else
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);// for win, ios, mac
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
#endif
    setGLBlend(GLBlendAlpha);
    glEnable(GL_BLEND);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glShadeModel(GL_SMOOTH);

    glFrontFace(GL_CCW);// GL_CW
    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepthf(1.0f);
#if 1
    glDepthFunc(GL_ALWAYS);
#endif

    setTexture(CLAMP, LINEAR);

#if 1// #add
    matrixProj = new iMatrix;
    matrixView = new iMatrix;
    vtx = new iVertex();

    fbo = new iFBO(devSize.width, devSize.height);
    texFboForiPopup = createTexture(devSize.width, devSize.height);

    //texStd = createTexture(32, 32);
    //fbo->bind(texStd);
    //setDotSize(32);
    //setRGBA(1, 1, 1, 1);
    //drawDot(16, 16);
    //fbo->unbind();
#endif
}

void freeOpenGL()
{
#if 1// #add
    delete matrixProj;
    delete matrixView;
    delete vtx;

    delete fbo;
    freeImage(texFboForiPopup);
#endif

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
}

static TextureWrap _wrap;
static TextureFilter _minFilter;
static TextureFilter _magFilter;

void getTexture(TextureWrap& wrap, TextureFilter& minFilter, TextureFilter& magFilter)
{
    wrap = _wrap;
    minFilter = _minFilter;
    magFilter = _magFilter;
}

void setTexture()
{
    setTexture(_wrap, _minFilter, _magFilter);
}

void setTexture(TextureWrap wrap, TextureFilter filter)
{
    if (filter != MIPMAP)
        setTexture(wrap, filter, filter);
    else
        setTexture(wrap, MIPMAP, LINEAR);
}

void setTexture(TextureWrap wrap, TextureFilter minFilter, TextureFilter magFilter)
{
    _wrap = wrap;
    _minFilter = minFilter;
    _magFilter = magFilter;

    GLenum e = (wrap == CLAMP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);

    if (minFilter != MIPMAP)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter == LINEAR ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter == LINEAR ? GL_LINEAR : GL_NEAREST);
    }
    else// if (minFilter == TextureFilterMipmap)
    {
        //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);// Deprecated
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void setTexture(GLuint texID, TextureWrap wrap, TextureFilter filter)
{
    glBindTexture(GL_TEXTURE_2D, texID);
    setTexture(wrap, filter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void setTexture(GLuint texID, TextureWrap wrap, TextureFilter minFilter, TextureFilter magFilter)
{
    glBindTexture(GL_TEXTURE_2D, texID);
    setTexture(wrap, minFilter, magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void readyOpenGL()
{
    glViewport(viewport.origin.x, viewport.origin.y,
        viewport.size.width, viewport.size.height);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 뿌리는 정점 균일할때
    matrixProj->loadIdentity();
    matrixProj->ortho(0, devSize.width, devSize.height, 0, -1, 1);

    matrixView->loadIdentity();
}

static GLBlend _blend = GLBlendAlpha;
void setGLBlend(GLBlend blend)
{
    switch (_blend = blend) {
    // dst = dst * (1.0f - src.a) + src * src.a
    case GLBlendAlpha: //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                            GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
    // dst = dst * 1.0f + src * src.a
    case GLBlendLight: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;

    case GLBlendMultiplied:
        //glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
        //    GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
        break;
    //default: break;
    }
}

GLBlend getGLBlend()
{
    return _blend;

    setRGBA(1, 0, 1, 0.3f);

    // (0*0.7 + 1*0.3,  0*0.7 + 0*0.3,  0*0.7 + 1*0.3, 1.0f + 0.3f)
    // (0.3,            0,              0.3,         , 0.3)
    // pre-multiplied alpha
}

// ---------------------------------------
// iVertex
// ---------------------------------------
iVertex* vtx;

iVertex::iVertex()
{
    id = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    int i, num = vertex_w_max * vertex_h_max;
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * 4 * num, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbe);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    //GLbyte indices[6] = { 0, 1, 2,   1, 2, 3 };
    GLushort* indices = new GLushort[num * 6];
    for (i = 0; i < num; i++)
    {
        indices[6 * i + 0] = 4 * i + 0;
        indices[6 * i + 1] = 4 * i + 1;
        indices[6 * i + 2] = 4 * i + 2;
        indices[6 * i + 3] = 4 * i + 1;
        indices[6 * i + 4] = 4 * i + 2;
        indices[6 * i + 5] = 4 * i + 3;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * num * 6, indices, GL_DYNAMIC_DRAW);
    delete indices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    num *= 4;
    vi = new VertexInfo[num];
    for (i = 0; i < num; i++)
    {
        vi[i].position[2] = 0;
        vi[i].position[3] = 1;
    }
    viNum = 1;
}

iVertex::~iVertex()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbe);

    delete vi;
}

struct ListProgram
{
    char* path;
    GLuint id;
};
static ListProgram* lp = NULL;
static int lpNum;

GLuint iVertex::getProgram(const char* strVert, const char* strFrag)
{
    if (lp == NULL)
    {
        lp = new ListProgram[100];
        lpNum = 0;
    }

    char s[256];
    sprintf(s, "%s%s", strVert, strFrag);
    for (int i = 0; i < lpNum; i++)
    {
        if (strcmp(s, lp[i].path) == 0)
            return lp[i].id;
    }

    int length;
     sprintf(s, "assets/shader/%s.vert", strVert);
    char* strCode = loadFile(s, length);
    GLuint vertID = createShaderID(strCode, shader_vert);
    delete strCode;

    sprintf(s, "assets/shader/%s.frag", strFrag);
    strCode = loadFile(s, length);
    GLuint fragID = createShaderID(strCode, shader_frag);
    delete strCode;

    GLuint programID = createProgramID(vertID, fragID);
    freeShaderID(vertID);
    freeShaderID(fragID);

    ListProgram* p = &lp[lpNum];
    lpNum++;
    sprintf(s, "%s%s", strVert, strFrag);
    p->path = new char[strlen(s) + 1];
    strcpy(p->path, s);
    return p->id = programID;
}

GLuint iVertex::useProgram(const char* strVert, const char* strFrag)
{
    GLuint programID = getProgram(strVert, strFrag);
    glUseProgram(programID);
    id = programID;
    return programID;
}

void iVertex::bufferSubData(VertexInfo* data, int w, int h)
{
    viNum = convert(data, w, h);
    if (data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexInfo) * 4 * viNum, vi);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

GLuint iVertex::enableVertexAttrArray(const char* str, const void* src, int stride, int size, int type)
{
    GLuint attr = glGetAttribLocation(id, str);
    glEnableVertexAttribArray(attr);
    glVertexAttribPointer(attr, size, type, GL_FALSE, stride, src);
    return attr;
}

void iVertex::disableVertexAttrArray(GLuint attr)
{
    glDisableVertexAttribArray(attr);
}

void iVertex::drawElements()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    glDrawElements(GL_TRIANGLES, 6 * viNum, GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void iVertex::setUniformMat(const char* str, iMatrix* m)
{
    GLuint loc = glGetUniformLocation(id, str);
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)m->d());
}
void iVertex::setUniform1f(const char* str, float x)
{
    GLuint loc = glGetUniformLocation(id, str);
    glUniform1f(loc, x);
}
void iVertex::setUniform2f(const char* str, float x, float y)
{
    GLuint loc = glGetUniformLocation(id, str);
    glUniform2f(loc, x, y);
}
void iVertex::setUniform1i(const char* str, int x)
{
    GLuint loc = glGetUniformLocation(id, str);
    glUniform1i(loc, x);
}


int iVertex::convert(VertexInfo* vtx, int w, int h)
{
    int num = w * h;
    int _num = vertex_w_max * vertex_h_max;
    if (num > _num)
    {
        memcpy(vi, vtx, sizeof(VertexInfo) * 4);
        return 1;
    }

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            for (int k = 0; k < 4; k++)
            {
                float x0 = 1.0f * (i + k % 2) / w;
                float x1 = 1.0f - x0;
                float y0 = 1.0f * (j + k / 2) / h;
                float y1 = 1.0f - y0;

                VertexInfo* vi = &this->vi[4 * w * j + 4 * i + k];
                vi->position[0] =   (vtx[0].position[0] * x1 + vtx[1].position[0] * x0) * y1+
                                    (vtx[2].position[0] * x1 + vtx[3].position[0] * x0) * y0;
                vi->position[1] =   (vtx[0].position[1] * x1 + vtx[1].position[1] * x0) * y1 +
                                    (vtx[2].position[1] * x1 + vtx[3].position[1] * x0) * y0;;
                vi->color.r =   (vtx[0].color.r * x1 + vtx[1].color.r * x0) * y1 +
                                (vtx[2].color.r * x1 + vtx[3].color.r * x0) * y0;
                vi->color.g =   (vtx[0].color.g * x1 + vtx[1].color.g * x0) * y1 +
                                (vtx[2].color.g * x1 + vtx[3].color.g * x0) * y0;
                vi->color.b =   (vtx[0].color.b * x1 + vtx[1].color.b * x0) * y1 +
                                (vtx[2].color.b * x1 + vtx[3].color.b * x0) * y0;
                vi->color.a =   (vtx[0].color.a * x1 + vtx[1].color.a * x0) * y1 +
                                (vtx[2].color.a * x1 + vtx[3].color.a * x0) * y0;
                vi->uv.x =  (vtx[0].uv.x * x1 + vtx[1].uv.x * x0) * y1 +
                            (vtx[2].uv.x * x1 + vtx[3].uv.x * x0) * y0;
                vi->uv.y =  (vtx[0].uv.y * x1 + vtx[1].uv.y * x0) * y1 +
                            (vtx[2].uv.y * x1 + vtx[3].uv.y * x0) * y0;
            }
        }
    }

    return w * h;
}

// ---------------------------------------
// Shader
// ---------------------------------------
// Compile
void showMessage(const char* title, const char* msg)
{
    wchar_t* wTitle = utf8_to_utf16(title);
    wchar_t* wMsg = utf8_to_utf16(msg);
    MessageBox(NULL, wMsg, wTitle, MB_OK);
    delete wTitle;
    delete wMsg;
}

void checkShaderID(GLuint id)
{
    GLint result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    int length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    if (result != GL_TRUE)
    {
        char* s = new char[length + 1];
        glGetShaderInfoLog(id, length, NULL, s);
        showMessage("checkShaderID error", s);
        delete s;
    }
}
GLuint createShaderID(const char* str, GLuint flag)
{
    GLuint f[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    GLuint id = glCreateShader(f[flag]);
    glShaderSource(id, 1, &str, NULL);
    glCompileShader(id);

    checkShaderID(id);

    return id;
}
void freeShaderID(GLuint id)
{
    glDeleteShader(id);
}

// Link
void checkProgramID(GLuint id)
{
    GLint result = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    int length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    if (result != GL_TRUE)
    {
        char* s = new char[length + 1];
        glGetProgramInfoLog(id, length, NULL, s);
        showMessage("checkProgramID error", s);
        delete s;
    }
}
GLuint createProgramID(GLuint vertID, GLuint fragID)
{
    GLuint id = glCreateProgram();
    glAttachShader(id, vertID);
    glAttachShader(id, fragID);
    glLinkProgram(id);
    glDetachShader(id, vertID);
    glDetachShader(id, fragID);

    checkProgramID(id);

    return id;
}
void freeProgramID(GLuint id)
{
    glDeleteProgram(id);
}


