#include <Windows.h>

//@param hInstance   ��ǰ���ڵľ��
//@param preInstance ǰһ��ʵ���ľ���������ò�����ȫ��ΪNULL
//@param cmdLine     ����������
//@param cmdShow     ����ģʽ��ID�ţ�Ҳ���������д��ڵ�״̬�����������С��..etc�� 
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