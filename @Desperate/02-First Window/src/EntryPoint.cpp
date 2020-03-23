// include the basic windows header file
#include <windows.h>
#include <windowsx.h>

// ���� WindowProcess �� ���ڴ�����Ϣ����callBack���ص������
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
    // Window class �Ļ�����Ϣ
    WNDCLASSEX wc;

    // ָ��WDNCLASSEX��Ҳ���ǰ����������Ĺ�ϵ
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // ����һЩ������Ϣ
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    // ע��class��Ϣ
    RegisterClassEx(&wc);

    // ����һ�����ڲ���hWndָ���������
    hWnd = CreateWindowEx(NULL,
        "WindowClass",    // window class name
        "Our First Windowed Program",   // ����
        WS_OVERLAPPEDWINDOW,    // ���ڷ��
        300,    // x-����
        300,    // y- ����
        500,    // ���
        400,    // �߶�
        NULL,    // ������, NULL
        NULL,    // �˵�, NULL
        hInstance,    // ����Handle
        NULL);    // �ര��, NULL

    // ��ʾ����
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:

    // ��Ϣ�����ݽṹ
    MSG msg;

    // ������Ϣ
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // ������Ϣ
        TranslateMessage(&msg);

        // ת��
        DispatchMessage(&msg);
    }

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// ��Ϣ�����handle�����ڴ�����Ϣ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // ������Ϣ���д���
    switch (message)
    {
    case WM_DESTROY: //���ڹر�
    {
        PostQuitMessage(0); //�ر�App
        return 0;
    } break;
    }

    // ������Ϣ��Ĭ�Ϲ�������������������Щû��switch�ж���Ĳ��ֵ�
    return DefWindowProc(hWnd, message, wParam, lParam);
}