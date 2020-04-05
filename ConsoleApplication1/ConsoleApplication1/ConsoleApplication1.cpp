#include <windows.h> 
#define X_COUNT           10
#define Y_COUNT           10
#define START_X           50 
#define START_Y           50 
#define INTERVAL          26 
#define HALF_INTERVAL     INTERVAL/2 
#define XPOS(x)   (START_X + (x) * INTERVAL) 
#define YPOS(y)   (START_Y + (y) * INTERVAL) 
int x, y;
int linkWhiteHorizontal = 0;
int linkBlackHorizontal = 0;
int linkWhiteDiagonal = 0;
int linkBlackDiagonal = 0;
unsigned char g_dol[Y_COUNT][X_COUNT];
unsigned char g_step;

LPCTSTR lpszClass = TEXT("유영진_17032049_오목게임");
void OnPaint(HWND ah_wnd)
{
    PAINTSTRUCT ps;
    HDC h_dc = BeginPaint(ah_wnd, &ps);

    for (int x = 0; x < X_COUNT; x++) {
        MoveToEx(h_dc, XPOS(x), YPOS(0), NULL);
        LineTo(h_dc, XPOS(x), YPOS(Y_COUNT - 1));
    }

    for (int y = 0; y < Y_COUNT; y++) {
        MoveToEx(h_dc, XPOS(0), YPOS(y), NULL);
        LineTo(h_dc, XPOS(X_COUNT - 1), YPOS(y));
    }

    for (int y = 0; y < Y_COUNT; y++) {
        for (int x = 0; x < X_COUNT; x++) {
            if (g_dol[y][x] > 0) {
                if (g_dol[y][x] == 1)
                {
                    SelectObject(h_dc, GetStockObject(BLACK_BRUSH));
                }
                else
                {
                    SelectObject(h_dc, GetStockObject(WHITE_BRUSH));
                }
                Ellipse(h_dc, XPOS(x) - HALF_INTERVAL, YPOS(y) - HALF_INTERVAL,
                    XPOS(x) + HALF_INTERVAL, YPOS(y) + HALF_INTERVAL);
            }
        }
    }

    EndPaint(ah_wnd, &ps);
}

void checkWin(HWND hWnd, int a_x, int a_y)
{
        int x = (a_x - START_Y + HALF_INTERVAL) / INTERVAL;
        int y = (a_y - START_Y + HALF_INTERVAL) / INTERVAL;
        if (g_dol[++y][x] == g_step)
        {
            linkWhiteHorizontal++;
        }
        else if (g_dol[++y][x] == !g_step)
        {
            linkBlackHorizontal++;
        }
    //else if(g_dol[y++][x]== !currentTurn)

    if (linkBlackHorizontal >= 5 || linkBlackDiagonal >= 5)
    {
        MessageBox(hWnd, "흑돌의 승리입니다", "게임종료", 0);
        PostQuitMessage(0);
    }
    else if (linkWhiteHorizontal >= 5 || linkBlackDiagonal >= 5)
    {
        MessageBox(hWnd, "백돌의 승리입니다", "게임종료", 0);
        PostQuitMessage(0);
    }
}
void OnLButtonDown(HWND ah_wnd, int a_x, int a_y)
{
    if (a_x > (XPOS(0) - HALF_INTERVAL) && a_y > (YPOS(0) - HALF_INTERVAL)
        && a_x < (XPOS(X_COUNT - 1) + HALF_INTERVAL)
        && a_y < (YPOS(Y_COUNT - 1) + HALF_INTERVAL)) {
        int x = (a_x - START_X + HALF_INTERVAL) / INTERVAL;
        int y = (a_y - START_Y + HALF_INTERVAL) / INTERVAL;
        if (g_dol[y][x] == 0) {
            g_dol[y][x] = g_step + 1;
            g_step = !g_step;
            InvalidateRect(ah_wnd, NULL, TRUE);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        OnPaint(hWnd);
        break;
    case WM_LBUTTONDOWN:
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        OnLButtonDown(hWnd, x, y);
        checkWin(hWnd, x, y);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc;
    HBRUSH h_bk_brush = CreateSolidBrush(RGB(244, 176, 77));

    wc.cbClsExtra = NULL;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)h_bk_brush;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = lpszClass;
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND h_wnd = CreateWindow(lpszClass, lpszClass,
        WS_OVERLAPPEDWINDOW, 50, 50, 350, 350, NULL, NULL, hInstance, NULL);
    ShowWindow(h_wnd, SW_SHOW);
    UpdateWindow(h_wnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(h_bk_brush);
    return (int)msg.wParam;
}