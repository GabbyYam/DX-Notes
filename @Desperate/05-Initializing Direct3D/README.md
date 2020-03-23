# COM？

- Component Object Model，用于生成高级对象（就像乐高一样）
  - 本质就是一个或一组通过接口控制的类
  - 每种COM对象都有一个id
  - 用Release()来结束一个COM对象
  - COM对象都是 “I” 打头的，比如ID3D10Divice



# Header

```c++
// windows 和 d3d 的headfile
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>     //很多主要的function
#include <d3dx11.h>    //前者的扩展包
#include <d3dx10.h>    //d3d11的许多declaration的来源

// d3d库
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

IDXGISwapChain *swapchain;     // swap chain
ID3D11Device *dev;             // Direct3D device 显卡的一些虚拟信息
ID3D11DeviceContext *devcon;   // Direct3D device context 上下文

// fuction declaration ：这两个相当于Constructor和Destructor
void InitD3D(HWND hWnd);     // 创建并初始化D3d对象
void CleanD3D(void);         // 销毁前者

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
```



# Launching

实质上要做的第一步，就是初始化COM对象

```cpp
//初始化COM对象
void InitD3D(HWND hWnd)
{
    // 保存swap chain信息的scd struct
    DXGI_SWAP_CHAIN_DESC scd;

    // 全部初始化为NULL
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // 补充 swap chain 的描述信息
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);
}
```

# 运行结果

结果还是什么都没有！！但好消息是其实我们的D3D对象已经在后台跑起来了

那为什么没有呢？因为我们还没渲染啊渲染，下一节马上开始，但是别着急，先想想下面的几个问题

![image-20200322161638791](README/image-20200322161638791.png)



# 几点问题

1. 我们怎么调 COM 对象的方法？为什么一定要release COM对象？
2. 硬件模式（hardware mode）和 引用模式（reference mode）的区别是什么？
3. device 和 device context的区别？