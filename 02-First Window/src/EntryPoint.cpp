// include the basic windows header file
#include <windows.h>
#include <windowsx.h>

// 定义 WindowProcess ， 用于处理消息，并callBack（回调）结果
LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

// entry point
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    // A window's Handle
    HWND hWnd;
    // Window class 的基本信息
    WNDCLASSEX wc;

    // 指定WDNCLASSEX，也就是绑定上面两个的关系
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // 设置一些基本信息
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    // 注册class信息
    RegisterClassEx(&wc);

    // 创建一个窗口并以hWnd指向这个窗口
    hWnd = CreateWindowEx(NULL,
        "WindowClass",    // window class name
        "Our First Windowed Program",   // 标题
        WS_OVERLAPPEDWINDOW,    // 窗口风格
        300,    // x-坐标
        300,    // y- 坐标
        500,    // 宽度
        400,    // 高度
        NULL,    // 父窗口, NULL
        NULL,    // 菜单, NULL
        hInstance,    // 程序Handle
        NULL);    // 多窗口, NULL

    // 显示窗口
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:

    // 消息的数据结构
    MSG msg;

    // 监听消息
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // 翻译消息
        TranslateMessage(&msg);

        // 转发
        DispatchMessage(&msg);
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// 消息处理的handle，用于处理消息
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 根据消息进行处理
    switch (message)
    {
    case WM_DESTROY: //窗口关闭
    {
        PostQuitMessage(0); //关闭App
        return 0;
    } break;
    }

    // 其余消息按默认规则处理，就是用来处理那些没在switch中定义的部分的
    return DefWindowProc(hWnd, message, wParam, lParam);
}