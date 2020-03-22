#include <Windows.h>

//@param hInstance   当前窗口的句柄
//@param preInstance 前一个实例的句柄，基本用不到，全程为NULL
//@param cmdLine     传入命令行
//@param cmdShow     窗口模式的ID号，也就是命令行窗口的状态（比如最大最小化..etc） 
// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    // create a "Hello World" message box using MessageBox()
    MessageBox(NULL,
        "Hello World!",
        "This is title~",
        MB_ICONEXCLAMATION | MB_OK);

    // return 0 to Windows
    return 0;
}