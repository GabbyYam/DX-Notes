本节正式开始初始化d3d组件，更新一下现在的结构，d3d组件将会归为GraphicsClass之下

![img](http://www.rastertek.com/pic0007.gif)

# D3DClass

先来看看结构，定义了很多东西，基本上整套写全了，以后就不需要那么麻烦了

```cpp
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

    //Lifecycle
	bool Initialize(int, int, bool, HWND, bool, float, float); //*****重点******
	void Shutdown();
	
    //渲染
	void BeginScene(float, float, float, float);
	void EndScene();

    //device 和 device context
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

    //变换矩阵
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

    //GPU信息
	void GetVideoCardInfo(char*, int&);

private:
	bool m_vsync_enabled;                            //GPU和显示器刷新同步
	int m_videoCardMemory;                           //显存
	char m_videoCardDescription[128];                //显卡信息
	IDXGISwapChain* m_swapChain;                     //swap chain
	ID3D11Device* m_device;                          //device
	ID3D11DeviceContext* m_deviceContext;            //device context
	ID3D11RenderTargetView* m_renderTargetView;      //渲染目标视图
	ID3D11Texture2D* m_depthStencilBuffer;           //深度缓冲
	ID3D11DepthStencilState* m_depthStencilState;    //深度缓冲状态
	ID3D11DepthStencilView* m_depthStencilView;      //深度缓冲视图
	ID3D11RasterizerState* m_rasterState;            //栅格状态
	D3DXMATRIX m_projectionMatrix;                   //投影矩阵
	D3DXMATRIX m_worldMatrix;                        //世界坐标变换
	D3DXMATRIX m_orthoMatrix;                        //正交矩阵
};

```

## 初始化:star:

初始化了很多东西啊..cover到了所有将来可能用到的东西，这里只贴用到的，没用到的暂时不说

- 参数：宽高，同步频率，hwnd，全屏，深度

  - 宽高就是窗口的宽和高
  - vsync：用于判断GPU是尽快刷新还是根据用户的显示器刷新频率 刷新
  - hwnd：窗口的句柄
  - depth & near：3D环境下的深度设定
  - fullscreen：是否全屏

- 取到显示器&GPU的刷新频率的分子分母计算刷新频率，为什么要这么做？

  如果不这么做，DX会默认采用blit而不是反转缓冲，这可能会导致性能下降 & 控制台疯狂报错

```cpp
bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear){
    //....Intialize...
    //...


    //取到分子分母
    // Create a DirectX graphics interface factory.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if(FAILED(result))
    {
        return false;
    }

    // Use the factory to create an adapter for the primary graphics interface (video card).
    result = factory->EnumAdapters(0, &adapter);
    if(FAILED(result))
    {
        return false;
    }

    // Enumerate the primary adapter output (monitor).
    result = adapter->EnumOutputs(0, &adapterOutput);
    if(FAILED(result))
    {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if(FAILED(result))
    {
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination.
    //创建一个list来保存显示器和GPU所有可能的显示模式组合
    displayModeList = new DXGI_MODE_DESC[numModes];
    if(!displayModeList)
    {
        return false;
    }

    // Now fill the display mode list structures.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if(FAILED(result))
    {
        return false;
    }

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    for(i=0; i<numModes; i++)
    {
        if(displayModeList[i].Width == (unsigned int)screenWidth)
        {
            if(displayModeList[i].Height == (unsigned int)screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    //取到分子分母后，把刚才申请的资源释放
    // Release the display mode list.
    delete [] displayModeList;
    displayModeList = 0;

    // Release the adapter output.
    adapterOutput->Release();
    adapterOutput = 0;

    // Release the adapter.
    adapter->Release();
    adapter = 0;

    // Release the factory.
    factory->Release();
    factory = 0;
    
    //Other things
    //.....
}


```

- swap chain初始化信息 swapChainDesc

```cpp
// Initialize the swap chain description.
ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

// Set to a single back buffer.
swapChainDesc.BufferCount = 1;

// Set the width and height of the back buffer.
swapChainDesc.BufferDesc.Width = screenWidth;
swapChainDesc.BufferDesc.Height = screenHeight;

// Set regular 32-bit surface for the back buffer.
swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

// Set the refresh rate of the back buffer.
if(m_vsync_enabled) // 设定刷新频率
{
    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
}
else               //否则是0，也就是最大速度刷新
{
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
}

// Set the usage of the back buffer.
swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

// Set the handle for the window to render to.
// 窗口句柄
swapChainDesc.OutputWindow = hwnd;

// Turn multisampling off.
swapChainDesc.SampleDesc.Count = 1;
swapChainDesc.SampleDesc.Quality = 0;

// Set to full screen or windowed mode.
// 是否全屏
if(fullscreen)
{
    swapChainDesc.Windowed = false;
}
else
{
    swapChainDesc.Windowed = true;
}


//这些是啥
// Set the scan line ordering and scaling to unspecified.
swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

// Discard the back buffer contents after presenting.
swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

// Don't set the advanced flags.
swapChainDesc.Flags = 0;
```

- 设定版本

```cpp
// Set the feature level to DirectX 11.
featureLevel = D3D_FEATURE_LEVEL_11_0;
```

- 创建swap chain，前面写好了基本信息，现在把它new出来

```cpp
// Create the swap chain, Direct3D device, and Direct3D device context.
result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
                                       D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
```

- 取到back buffer的地址，并为bb创建rt

```cpp
// Get the pointer to the back buffer.
result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

// Create the render target view with the back buffer pointer.
result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
if(FAILED(result))

// Release pointer to the back buffer as we no longer need it.
backBufferPtr->Release();
backBufferPtr = 0;
```

- 初始化三连 depth buffer

```cpp
// Initialize the description of the depth buffer.
ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

// Set up the description of the depth buffer.
depthBufferDesc.Width = screenWidth;
depthBufferDesc.Height = screenHeight;
depthBufferDesc.MipLevels = 1;
depthBufferDesc.ArraySize = 1;
depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
depthBufferDesc.SampleDesc.Count = 1;
depthBufferDesc.SampleDesc.Quality = 0;
depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
depthBufferDesc.CPUAccessFlags = 0;
depthBufferDesc.MiscFlags = 0;

// Create the texture for the depth buffer using the filled out description.
result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
```

- 初始化三连 depthStencil

```cpp
// Initialize the description of the stencil state.
ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

// Set up the description of the stencil state.
depthStencilDesc.DepthEnable = true;
depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

depthStencilDesc.StencilEnable = true;
depthStencilDesc.StencilReadMask = 0xFF;
depthStencilDesc.StencilWriteMask = 0xFF;

// Stencil operations if pixel is front-facing.
depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

// Stencil operations if pixel is back-facing.
depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

// Create the depth stencil state.
result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

// Set the depth stencil state.
m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
```

- 初始化三连 depth stencil view 

```cpp
// Initailze the depth stencil view.
ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

// Set up the depth stencil view description.
depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
depthStencilViewDesc.Texture2D.MipSlice = 0;

// Create the depth stencil view.
result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

// Bind the render target view and depth stencil buffer to the output render pipeline.
m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
```

- 现在我们基本的render target就准备好了，下面继续准备一些未来可能用到的extra function
- 首先是栅格状态，虽然不做任何设置DX也会为我们自动创建一个，但是那样就没法对其进行控制了

```cpp
// Setup the raster description which will determine how and what polygons will be drawn.
rasterDesc.AntialiasedLineEnable = false;
rasterDesc.CullMode = D3D11_CULL_BACK;
rasterDesc.DepthBias = 0;
rasterDesc.DepthBiasClamp = 0.0f;
rasterDesc.DepthClipEnable = true;
rasterDesc.FillMode = D3D11_FILL_SOLID;
rasterDesc.FrontCounterClockwise = false;
rasterDesc.MultisampleEnable = false;
rasterDesc.ScissorEnable = false;
rasterDesc.SlopeScaledDepthBias = 0.0f;

// Create the rasterizer state from the description we just filled out.
result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);

// Now set the rasterizer state.
m_deviceContext->RSSetState(m_rasterState);
```

- Viewport，设为整个窗口的大小

```cpp
// Setup the viewport for rendering.
viewport.Width = (float)screenWidth;
viewport.Height = (float)screenHeight;
viewport.MinDepth = 0.0f;
viewport.MaxDepth = 1.0f;
viewport.TopLeftX = 0.0f;
viewport.TopLeftY = 0.0f;

// Create the viewport.
m_deviceContext->RSSetViewports(1, &viewport);
```

- 投影矩阵，将3d场景投影到上面的Viewport space

```cpp
// Setup the projection matrix.
fieldOfView = (float)D3DX_PI / 4.0f;
screenAspect = (float)screenWidth / (float)screenHeight;

// Create the projection matrix for 3D rendering.
D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
```

- world matrix，将model的坐标映射到世界坐标上，这里就映射成自己就行了，相当于没变

```cpp
// Initialize the world matrix to the identity matrix.
D3DXMatrixIdentity(&m_worldMatrix);
```

- 视图变换，相当于摄像机的视图变换，下一节会把它移到一个cameraClass中，所以这里暂时不用管
- 正交变换，用来帮助我们渲染3d中的2d部分，比如用户接口，后面会演示它的作用

```cpp
// Create an orthographic projection matrix for 2D rendering.
D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
```

# 销毁

逐个release，不多说了

## 渲染

- 选定 rt （RenderTarget）和 ds（DepthStencil）

```cpp
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// 初始化color内存，用于渲染back buffer 也就是 背景画布
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

    // bb的rt对象设为当前rt
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    
	// 选定深度缓冲
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}
```

- swapchain 交换 font buffer 和 back buffer 更新屏幕显示的结果

```cpp
void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_vsync_enabled)
	{
		// 固定屏幕刷新频率
		m_swapChain->Present(1, 0);
	}
	else
	{
		// 否则尽快刷新
		m_swapChain->Present(0, 0);
	}

	return;
}
```



# Gotcha！

如果运行成功，我们会得到一个带颜色的窗口，因为我们目前什么都没有，只渲染了一个2d纯色贴图

你也可以尝试以下目标作为练习：

- 将窗口调整为全屏
- 将2d纯色贴图渲染成黄色
- 将显卡名和显存输出到一个文本文件中

![image-20200323235527291](README/image-20200323235527291.png)