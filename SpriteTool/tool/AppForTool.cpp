// SpriteTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "AppForTool.h"
#include "Game.h"
#include "SpriteTool.h"

#define MAX_LOADSTRING 100

WCHAR szTitle[100] = TEXT("SpriteTool");
WCHAR szWindowClass[100] = TEXT("SpriteTool");
HINSTANCE hInst;
HWND hWnd;
HDC hDC;
bool runWnd;
bool isFullscreen;
void mainLoop();
void updateWindow();

HWND hWndStatic;

HWND hWndBtn;
HWND hWndCheckBox;
HWND* hWndRadio;

HWND hWndCombBox;
HWND hWndListBox;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITETOOL));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_PORTFOLIOGDIPLUS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    hInst = hInstance;
    int x = -1000000, y = -1000000, width = DEV_WIDTH, height = DEV_HEIGHT;
    DWORD style = //WS_OVERLAPPEDWINDOW;
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_THICKFRAME |
        WS_MINIMIZEBOX |
        WS_MAXIMIZEBOX;
    hWnd = CreateWindow(szWindowClass, szTitle, style,
        x, y, width, height, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL)
        return FALSE;
    isFullscreen = false;
 //   hDC = GetDC(hWnd);
 //   loadOpenGL(hDC);
  //
//    startApp(hDC, loadGame);
    initWndCtrlSystem();

    loadCursor();
    ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);
    updateWindow();
    loadSpriteTool();

#if 1
    DragAcceptFiles(hWnd, TRUE);/////////////////////

#endif

    MSG msg;
    for (runWnd = true; runWnd; )
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mainLoop();
        }
    }

    //freeOpenGL();
    ReleaseDC(hWnd, hDC);
    freeCursor();
    endApp(freeGame);
    return (int)msg.wParam;
}

void mainLoop()
{
    //glClearColor(1, 0, 0, 1);
    //glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
    //SwapBuffers(hDC);
    //return;
#if 0
    readyOpenGL();
    drawApp(drawGame);
    //glFlush();// glFinish()
    SwapBuffers(hDC);
#else
    if (fbo == NULL)
        return;
    fbo->bind();
    //glViewport(0, 0, devSize.width, devSize.height);
    glViewport(0, 0, fbo->tex->width, fbo->tex->height);
    //glViewport(0, 0, 320, 240);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawApp(drawGame);
    // 원래는 여기 있는게 맞음.
    //drawCursor(iFPS::instance()->lastDt);
    fbo->unbind();
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height); 
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setGLBlend(GLBlendMultiplied);
    STD::setRGBA(1, 1, 1, 1);

    iPoint p = iPointZero;
    float s = updateZoom(iFPS::instance()->lastDt, p);

    Texture* t = fbo->tex;
    STD::drawImage(t, p.x, p.y, TOP | LEFT,
        0, 0, t->width, t->height, s, s, 2, 0, REVERSE_HEIGHT);
    setGLBlend(GLBlendAlpha);
    //glFlush();// glFinish()
    SwapBuffers(hDC);
#endif
}


iSize sizeMonitor;
static int win_border_width = 0, win_border_height = 0;

void updateWindow()
{
    RECT rect;
    GetWindowRect(hWnd, &rect);
    //enforceResolution(0, rect, win_border_width, win_border_height);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    int x = (sizeMonitor.width - w) / 2;
    int y = (sizeMonitor.height - h) / 2;

    SetWindowPos(hWnd, HWND_TOP,
        x, y, w + win_border_width, h + win_border_height, SWP_SHOWWINDOW);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
#if 0
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;

        wchar_t wstrPath[1024];
        DragQueryFile(hDrop, 0, wstrPath, 1024);

        char* str = utf16_to_utf8(wstrPath);
        printf("drag = [%s]\n", str);
        //if (strcmp(&str[strlen(str) - 4], "txt") != 0)
        //{
        //    MessageBox(NULL, L"Not support", L"Not support", MB_OK);
        //}
        //else
        //{
        //    // do something
        //}
        delete str;

        //showChooseColor(NULL);
        const char* path = openFileDlg(false, L"All Files(*.*)\0*.*\0Text(*.txt)\0*.txt;.TXT\0");
        printf("open = %s\n", path);
        break;
    }

    case WM_CTLCOLORSTATIC:// static/checkbox/radio/group
    //case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:// editbox/combobox
    //case WM_CTLCOLORLISTBOX:
    {
        break;
    }

    case WM_COMMAND:
    {
        HWND hwnd = (HWND)lParam;

        if (hwnd == hWndCombBox)
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                printf("comboBox %d\n", indexWndComboBox(hWndCombBox));
                break;
            }
        }

        if (hwnd == hWndListBox)
        {
            if (HIWORD(wParam) == LBN_SELCHANGE)
            {
                printf("listBox sel %d\n", indexWndListBox(hWndListBox));
                break;
            }
            else if (HIWORD(wParam) == LBN_DBLCLK)
            {
                printf("listBox dbl %d\n", indexWndListBox(hWndListBox));
                break;
            }
        }

        if (hwnd == hWndBtn)
        {
            printf("btn push\n");
        }
        else if (hwnd == hWndCheckBox)
        {
            printf("btn checkbox\n");
            bool on = getWndCheckBox(hwnd);
            setWndCheckBox(hwnd, !on);
            enableWnd(hWndBtn, on);
            setWndText(hWndBtn, on ? "확인" : "OK");
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                if (hwnd == hWndRadio[i])
                {
                    printf("btn radio[%d]\n", i);
                    setWndRadio(hwnd, true);
                    for (int j = 0; j < 3; j++)
                    {
                        if (j != i)
                            setWndRadio(hWndRadio[j], false);
                    }
                    break;
                }
            }
        }
        break;
    }
#endif
    case WM_COMMAND:
    {
        updateSpriteTool(wParam, lParam);
        break;
    }

    case WM_CREATE:
    {
        xprintf("wm_create\n");
        // 640 x 480
        // 4000 x 3000 => 3000 : 2250
        // (3000 + win_border_width), (2250 + win_border_height)

        sizeMonitor = iSizeMake(GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN));
        printf("monitor(%.f, %.f)\n", sizeMonitor.width, sizeMonitor.height);

        RECT rt;
        GetWindowRect(hWnd, &rt); printf("wrt(%d, %d, %d, %d)\n", rt.left, rt.top, rt.right, rt.bottom);
        RECT rtWnd = rt;
        GetClientRect(hWnd, &rt); printf("crt(%d, %d, %d, %d)\n", rt.left, rt.top, rt.right, rt.bottom);
        win_border_width = (rtWnd.right - rtWnd.left) - (rt.right - rt.left);
        win_border_height = (rtWnd.bottom - rtWnd.top) - (rt.bottom - rt.top);
        //GetSystemMetrics(SM_CXFRAME) * 2;// == win_border_width
        //GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);// == win_border_height
        break;
    }

    case WM_GETMINMAXINFO:
    {
        if (win_border_width == 0 || win_border_height == 0)
            break;

        int minWidth = sizeMonitor.width * RATE_MONITOR;
        int minHeight = minWidth * (devSize.height / devSize.width);

        MINMAXINFO* info = (MINMAXINFO*)lParam;
        info->ptMinTrackSize.x = minWidth + win_border_width;
        info->ptMinTrackSize.y = minHeight + win_border_height;
        break;
    }

    case WM_SIZING:
    case WM_MOVING:
    {
        break;// test 000

        //RECT rt;
        //GetWindowRect(hWnd, &rt);
        RECT& rect = *reinterpret_cast<LPRECT>(lParam);
        enforceResolution((int)wParam, rect, win_border_width, win_border_height);
        resizeApp(rect.right - rect.left - win_border_width,
            rect.bottom - rect.top - win_border_height);
        mainLoop();
        return 0;// break;
    }

    case WM_SIZE:
    {
         break;// test 000

        //RECT rt;
        //GetClientRect(hWnd, &rt);
        resizeApp(LOWORD(lParam), HIWORD(lParam));
        mainLoop();
        return 0;// break;
    }

    case WM_LBUTTONDOWN:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatBegan, cursor);
        //setZoom(cursor, 3.0f, 0.5f, 1.0f);
        //iFPS::instance()->setSlowTime(1.2f, 0.1f);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatMoved, cursor);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatEnded, cursor);
        return 0;
    }

    //case WM_CHAR:
    //    return 0;// break;
    case WM_KEYDOWN:
    {
        setKeyStat(iKeyStatBegan, wParam);
        setKeyDown(iKeyStatBegan, wParam);
        return 0;// break;
    }

    case WM_KEYUP:
    {
        if (wParam == VK_ESCAPE)
        {
            goFullscreen();
            return 0;
        }
        setKeyStat(iKeyStatEnded, wParam);
        setKeyDown(iKeyStatEnded, wParam);
        return 0;// break;
    }

    case WM_SETCURSOR:
    {
        if (updateCursor(LOWORD(lParam) == HTCLIENT))
            return true;
        break;
    }

    case WM_CLOSE:
    {
        const char* s0[3] = {
            "Crazy?",
            "툴을 중료하시겠습니까??",
            "あなた醜い？"
        };

        const char* s1[3] = {
            "Really?",
            "종료",
            "本物？"
        };

        int language = 1;// en:0, kr:1, jp:2

        wchar_t* ws0 = utf8_to_utf16(s0[language]);
        wchar_t* ws1 = utf8_to_utf16(s1[language]);

        //if (MessageBox(NULL, TEXT("너 못생겼어?"), TEXT("진짜?"), MB_YESNO) == IDYES)
        if (MessageBox(NULL, ws0, ws1, MB_YESNO) == IDYES)
        {
            runWnd = false;
        }

        delete ws0;
        delete ws1;
        return 0;// break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;// break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void setCurrentMonitor(RECT& rt)
{
    GetWindowRect(hWnd, &rt);
    HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hMonitor, &mi);
    if (mi.dwFlags == MONITORINFOF_PRIMARY)
    {
        rt.left = 0;
        rt.right = sizeMonitor.width;
        rt.top = 0;
        rt.bottom = sizeMonitor.height;
    }
    else
    {
        memcpy(&rt, &mi.rcWork, sizeof(RECT));
    }
}

RECT rtPrev;
void goFullscreen()
{
    isFullscreen = !isFullscreen;
    if (isFullscreen)
    {
        GetWindowRect(hWnd, &rtPrev);
        RECT rt;
        setCurrentMonitor(rt);
        int x = rt.left,
            y = rt.top,
            w = rt.right - rt.left,
            h = rt.bottom - rt.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP,
            x, y, w, h,
            SWP_SHOWWINDOW);
    }
    else
    {
        int x = rtPrev.left,
            y = rtPrev.top,
            w = rtPrev.right - rtPrev.left,
            h = rtPrev.bottom - rtPrev.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP,
            x, y, w, h, SWP_SHOWWINDOW);
    }
}

void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height)
{
    switch (edge)
    {
    case WMSZ_BOTTOM:
    case WMSZ_TOP:
    {
        // w : h = devSize.width : devSize.height
        int h = (rect.bottom - rect.top) - win_border_height;// client width
        int w = h * devSize.width / devSize.height + win_border_width;// window_width
        rect.left = (rect.left + rect.right) / 2 - w / 2;
        rect.right = rect.left + w;
        break;
    }
    case WMSZ_BOTTOMLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_BOTTOMRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        // w : h = devSize.width : devSize.height
        int w = (rect.right - rect.left) - win_border_width;
        int h = w * devSize.height / devSize.width + win_border_height;
        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
        rect.bottom = rect.top + h;
        break;
    }
    case WMSZ_TOPLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    case WMSZ_TOPRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    }
}

Texture* texCursor;
iPoint cursor;
bool bCursor;
void loadCursor()
{
    return;
    texCursor = createImage("assets/cursor.png");

    cursor = iPointZero;
    bCursor = false;
}

void freeCursor()
{
    return;
    freeImage(texCursor);
}

void drawCursor(float dt)
{
    return;
    if (bCursor)
        STD::drawImage(texCursor, cursor.x, cursor.y, TOP | LEFT);
}

bool updateCursor(bool inClient)
{
    return false;

    if (bCursor == inClient)
        return false;

    bCursor = inClient;
#if 0
    ShowCursor(bCursor ? FALSE : TRUE);
#else
    if (bCursor)
    {
        for (;;)
        {
            int n = ShowCursor(FALSE);
            if (n < 0) break;
        }
    }
    else
    {
        for (;;)
        {
            int n = ShowCursor(TRUE);
            if (n > -1) break;
        }
    }
#endif
    return true;
}

