#include "iWindow.h"
#include "iStd.h"

void startApp(HDC hdc, VOID_METHOD methodLoad)
{
    devSize.width = DEV_WIDTH;
    devSize.height = DEV_HEIGHT;

    iGraphics::share()->start(hdc);
    loadOpenGL(hdc);

    fbo = new iFBO(devSize.width, devSize.height);
    texFboForiPopup = createTexture(devSize.width, devSize.height);
    appInitialize();
    methodLoad();// loadGame
}

void endApp(VOID_METHOD methodFree)
{
    methodFree();// freeGame

    iGraphics::share()->end();
    freeOpenGL();
}

void drawApp(FLOAT_METHOD methodDraw)
{
#if 0
    int f = iFPS::instance()->framesPerSec;
#endif

    float dt = iFPS::instance()->update();
    //if( dt > 0.001 )
    //xprintf("drawGame %f %d\n", dt, f);

    // binding fbo
    methodDraw(dt);// drawGame

    keyDown = 0;

#if 1// 원래는 App.cpp 56 라인에 있어야 함.
    extern void drawCursor(float dt);
    drawCursor(iFPS::instance()->lastDt);
#endif
}

void resizeApp(int width, int height)
{
    float rx = width / devSize.width;
    float ry = height / devSize.height;
    if (rx < ry)
    {
        viewport.origin.x = 0;
        viewport.size.width = width;

        viewport.size.height = devSize.height * rx;
        viewport.origin.y = (height - viewport.size.height) / 2;
    }
    else
    {
        viewport.origin.y = 0;
        viewport.size.height = height;

        viewport.size.width = devSize.width * ry;
        viewport.origin.x = (width - viewport.size.width) / 2;
    }

    //xprintf("한글 devSize(%.f,%.f), real(%d, %d), viewport(%.f,%.f,%.f,%.f)\n",
    //    devSize.width, devSize.height,
    //    width, height,
    //    viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

// 100 x 50 dev
// 200 x 100 resolution
iPoint coordinate(int x, int y)
{
    float r = devSize.width / viewport.size.width;
    //float r = devSize.height / viewport.size.height;
    iPoint p;
    p.x = (x - viewport.origin.x) * r;
    p.y = (y - viewport.origin.y) * r;

    return p;
}

#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "imm32")
#pragma comment(lib, "Comctl32")

extern HWND hWnd;

void initWndCtrlSystem()
{
#if 1
    InitCommonControls(); // 6.0 or later
#else
    INITCOMMONCONTROLSEX picce;
    picce.dwSize = sizeof(INITCOMMONCONTROLSEX);
    picce.dwICC = ICC_STANDARD_CLASSES | ICC_UPDOWN_CLASS;
    InitCommonControlsEx(&picce); // 4.3 or later
#endif

    // do something ...
}

HWND createWndStatic(const char* str, int x, int y, int width, int height, Method_Color_Update mColor)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_STATIC, s, WS_CHILD | WS_VISIBLE | ES_CENTER,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    delete s;

    return hwnd;
}

HWND createWndButton(const char* str, int x, int y, int width, int height)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_BUTTON, s, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    delete s;

    return hwnd;
}

HWND createWndCheckBox(const char* str, int x, int y, int width, int height)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_BUTTON, s, WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    delete s;

    return hwnd;
}

bool getWndCheckBox(HWND hwnd)
{
    return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

void setWndCheckBox(HWND hwnd, bool on)
{
    SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);
}

HWND createWndComboBox(const char** str, int strNum, int x, int y, int width, int height)
{
    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_COMBOBOX, NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_HASSTRINGS,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    addWndComboBox(hwnd, str, strNum);
    setWndComboBox(hwnd, strNum);

    return hwnd;
}

void addWndComboBox(HWND hwnd, int index, const char* str)
{
    wchar_t* s = utf8_to_utf16(str);
    SendMessage(hwnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)s);
    delete s;
}

void addWndComboBox(HWND hwnd, const char** str, int strNum)
{
    for (int i = 0; i < strNum; i++)
        addWndComboBox(hwnd, i, str[i]);
}

void removeWndComboBox(HWND hwnd, int index)
{
    SendMessage(hwnd, CB_DELETESTRING, (WPARAM)index, (LPARAM)0);
}

int countWndComboBox(HWND hwnd)
{
    return SendMessage(hwnd, CB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}

int indexWndComboBox(HWND hwnd)
{
    return SendMessage(hwnd, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

void setWndComboBox(HWND hwnd, int index)
{
    SendMessage(hwnd, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}

char* getWndComboBox(HWND hwnd, int index)
{
    wchar_t wstr[128];
    SendMessage(hwnd, CB_GETLBTEXT, (WPARAM)index, (LPARAM)wstr);

    return utf16_to_utf8(wstr);
}

HWND createWndListBox(const char** str, int strNum, int x, int y, int width, int height)
{
    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_LISTBOX, NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_HSCROLL | WS_VSCROLL,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    addWndListBox(hwnd, str, strNum);
    setWndListBox(hwnd, strNum);

    return hwnd;
}

void addWndListBox(HWND hwnd, int index, const char* str)
{
    wchar_t* s = utf8_to_utf16(str);
    SendMessage(hwnd, LB_INSERTSTRING, (WPARAM)index, (LPARAM)s);
    delete s;
}

void addWndListBox(HWND hwnd, const char** str, int strNum)
{
    for (int i = 0; i < strNum; i++)
        addWndListBox(hwnd, i, str[i]);
}

void removeWndListBox(HWND hwnd, int index)
{
    SendMessage(hwnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
}

int countWndListBox(HWND hwnd)
{
    return SendMessage(hwnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}

int indexWndListBox(HWND hwnd)
{
    return SendMessage(hwnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

void setWndListBox(HWND hwnd, int index)
{
    SendMessage(hwnd, LB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}

char* getWndListBox(HWND hwnd, int index)
{
    wchar_t wstr[128];
    SendMessage(hwnd, LB_GETTEXT, (WPARAM)index, (LPARAM)wstr);

    return utf16_to_utf8(wstr);
}

HWND createWndRadio(const char* str, int x, int y, int width, int height)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_BUTTON, s, WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    delete s;

    return hwnd;
}

bool getWndRadio(HWND hwnd)
{
    return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

void setWndRadio(HWND hwnd, bool on)
{
    SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);

}

HWND createWndGroupBox(const char* str, int x, int y, int width, int height)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_BUTTON, s, WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);

    delete s;

    return hwnd;
}

LRESULT CALLBACK editWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, WndEditStyle style)
{
    WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (wpOld)
    {
        switch (msg) {

        case WM_ERASEBKGND:
            break;

        case WM_NCDESTROY:
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
            break;

        case WM_CHAR:
            // VK_DELETE
            if (style == WndEditAll)
            {
                // ok
            }
            else if (style == WndEditInt)
            {
                if (wParam == VK_RETURN || wParam == VK_BACK ||
                    wParam == '-' || (wParam >= '0' && wParam <= '9'))
                {
                    // ok
                }
                else
                    return 0;
            }
            else
            {
                if (wParam == '.')
                {
                    wchar_t str[1024];
                    int length = GetWindowTextLength(hwnd) + 1;
                    GetWindowText(hwnd, str, length);
                    for (int i = 0; i < length; i++)
                    {
                        if (str[i] == '.')
                            return 0;
                    }
                }
                else if (wParam == VK_RETURN || wParam == VK_BACK ||
                    wParam == '-' || (wParam >= '0' && wParam <= '9'))
                {
                    // ok
                }
                else
                    return 0;
            }
            break;
        }
    }

    return CallWindowProc(wpOld, hwnd, msg, wParam, lParam);
}
LRESULT CALLBACK editAllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return editWndProc(hWnd, message, wParam, lParam, WndEditAll);
}
LRESULT CALLBACK editIntWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return editWndProc(hWnd, message, wParam, lParam, WndEditInt);
}

LRESULT CALLBACK editFloatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return editWndProc(hWnd, message, wParam, lParam, WndEditFloat);
}


int strLegnthMax = 0;
HWND createWndEditBox(const char* str, int x, int y, int width, int height, WndEditStyle style)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_EDIT, s,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);
    delete s;

    if (strLegnthMax)
        SendMessage(hwnd, (UINT)EM_LIMITTEXT, (WPARAM)strLegnthMax, (LPARAM)0);
    LONG parm = GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, parm);

    typedef LRESULT(CALLBACK* EDIT_METHOD)(HWND, UINT, WPARAM, LPARAM);
    EDIT_METHOD method[3] = {
        editAllWndProc,
        editIntWndProc,
        editFloatWndProc
    };

    SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)method[style]);

    return hwnd;
}

LRESULT CALLBACK editMultilineWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, WndEditStyle style)
{
    WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (wpOld)
    {
        switch (msg) {

        case WM_ERASEBKGND:
            break;

        case WM_NCDESTROY:
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
            break;

        case WM_GETDLGCODE:
        {
            LRESULT ret = CallWindowProc(wpOld, hwnd, msg, wParam, lParam);
            ret &= ~(DLGC_HASSETSEL | DLGC_WANTTAB);
            if (lParam &&
                ((LPMSG)lParam)->message == WM_KEYDOWN &&
                ((LPMSG)lParam)->wParam == VK_TAB)
                ret &= -DLGC_WANTMESSAGE;

            return ret;
        }
        }
    }

    return CallWindowProc(wpOld, hwnd, msg, wParam, lParam);
}

HWND createWndEditBoxMultiline(const char* str, int x, int y, int width, int height)
{
    wchar_t* s = utf8_to_utf16(str);

    HWND hwndParent = hWnd;
    HINSTANCE instanceParent = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
    HWND hwnd = CreateWindow(WC_EDIT, s,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_MULTILINE |
        ES_WANTRETURN | ES_NOHIDESEL | ES_AUTOVSCROLL | WS_VSCROLL,
        x, y, width, height, hwndParent, (HMENU)0, instanceParent, NULL);
    delete s;

    if (strLegnthMax)
        SendMessage(hwnd, (UINT)EM_LIMITTEXT, (WPARAM)strLegnthMax, (LPARAM)0);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)editMultilineWndProc);

    return hwnd;
}

// ===================================================
//	Common
// ===================================================
void enableWnd(HWND hwnd, bool enable)
{
    EnableWindow(hwnd, enable);
}

void setWndText(HWND hwnd, const char* szFormat, ...)
{
    char szText[1024];
    va_start_end(szText, szFormat);

    wchar_t* s = utf8_to_utf16(szText);
    SetWindowText(hwnd, s);
    delete s;
}

char* getWndText(HWND hwnd)
{
    wchar_t str[1024];
    int length = GetWindowTextLength(hwnd) + 1;
    GetWindowText(hwnd, str, length);
    return utf16_to_utf8(str);
}

int getWndInt(HWND hwnd)
{
    char* str = getWndText(hwnd);
    int n = atoi(str);
    delete str;
    return n;
}

float getWndFloat(HWND hwnd)
{
    char* str = getWndText(hwnd);
    float n = atof(str);
    delete str;
    return n;
}

// ===================================================
//	FileDlg
// ===================================================
static char* strOpenPath = NULL;
bool flag = false;
wchar_t dir[512];
wchar_t _dir[512];

const char* openFileDlg(bool open, LPCWSTR filter)
{
    const wchar_t* path = L"\\assets\\stagedata";
    GetCurrentDirectory(512, _dir);
    GetCurrentDirectory(512, dir);
    wcscat(dir, path);
    char* sPath = utf16_to_utf8(dir);
    printf("%s\n", sPath);
    delete sPath;

    wchar_t wstrPath[1024];

    OPENFILENAME ofn;
    memset(&ofn, 0x00, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = dir;
    ofn.lpstrFile = wstrPath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    bool success = false;
    if (open) success = GetOpenFileName(&ofn);
    else success = GetSaveFileName(&ofn);

    if (success)
    {
        SetCurrentDirectory(_dir);

        wsprintf(wstrPath, TEXT("%s"), ofn.lpstrFile);
        if (strOpenPath)
            delete strOpenPath;
        strOpenPath = utf16_to_utf8(wstrPath);
        return strOpenPath;
    }

    return NULL;
}


const char* openFileDlg(bool open, LPCWSTR filter, const wchar_t* _path)
{    
    const wchar_t* path = _path;
    GetCurrentDirectory(512, _dir);
    GetCurrentDirectory(512, dir);
    wcscat(dir, path);
    char* sPath = utf16_to_utf8(dir);
    printf("%s\n", sPath);
    delete sPath;

    wchar_t wstrPath[1024];
    
    OPENFILENAME ofn;
    memset(&ofn, 0x00, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = dir;
    ofn.lpstrFile = wstrPath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    bool success = false;
    if (open) success = GetOpenFileName(&ofn);
    else success = GetSaveFileName(&ofn);

    if (success)
    {
        SetCurrentDirectory(_dir);

        wsprintf(wstrPath, TEXT("%s"), ofn.lpstrFile);
        if (strOpenPath)
            delete strOpenPath;
        strOpenPath = utf16_to_utf8(wstrPath);
        return strOpenPath;
    }

    return NULL;
}

const char* openFileDlg(bool open, const char* filter)
{
    wchar_t* wFilter = utf8_to_utf16(filter);
    const char* path = openFileDlg(open, wFilter);
    delete wFilter;

    return path;
}

// ===================================================
//	ChooseColor
// ===================================================
void showChooseColor(CHOOSE_COLOR method)
{
    CHOOSECOLOR cc;
    memset(&cc, 0x00, sizeof(CHOOSECOLOR));
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hWnd;
    COLORREF color[16];
    cc.lpCustColors = color;
    cc.Flags = CC_RGBINIT | CC_FULLOPEN;
    if (!ChooseColor(&cc))
        return;

    int r = (cc.rgbResult & 0x0000ff);
    int g = (cc.rgbResult & 0x00ff00) >> 8;
    int b = (cc.rgbResult & 0xff0000) >> 16;

    printf("(%d, %d, %d)\n", r, g, b);
    if (method)
        method(r / 255.f, g / 255.f, b / 255.f);
}


// ===================================================
//	Thread
// ===================================================
#include <process.h>

struct ThreadInf
{
    MethodThread method;
    void* parm;
};

ThreadInf* ti = NULL;

unsigned int __stdcall thread_func(void* parm)
{
    ThreadInf* ti = (ThreadInf*)parm;
    ti->method(ti->parm);
    delete ti;

    _endthreadex(0);
    return 0;
}

HANDLE startThread(MethodThread method, void* parm)
{
    if (ti == NULL)
        ti = new ThreadInf;
    ti->method = method;
    ti->parm = parm;

    unsigned int id;
    return (HANDLE)_beginthreadex(NULL, 0, thread_func, ti, NULL, &id);
}

void waitUntilThread(HANDLE* handle, int numHandle)
{
    if (numHandle == 1)
        WaitForSingleObject(handle[0], INFINITE);
    else
        WaitForMultipleObjects(numHandle, handle, TRUE, INFINITE);
}


// ===================================================
//	iCriticalSection
// ===================================================
static iCriticalSection* instance = NULL;
iCriticalSection::iCriticalSection()
{
    InitializeCriticalSection(&cs);
}
iCriticalSection* iCriticalSection::share()
{
    if (instance == NULL)
        instance = new iCriticalSection();
    return instance;
}
iCriticalSection::~iCriticalSection()
{
    DeleteCriticalSection(&cs);

    instance = NULL;
}

void iCriticalSection::start()
{
    EnterCriticalSection(&cs);
}
void iCriticalSection::end()
{
    LeaveCriticalSection(&cs);
}





