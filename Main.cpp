//The include order is important 
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>
#include <DirectXMath.h>
#define XM_NO_INTRINSICS_   //Use less optimal but more compatible code 
#define XM_NO_ALLIGNMENT_
#include <dinput.h>
#include <sstream>
#include "Camera.h"
#include "Text2D.h" 
#include "Model.h"
#include "App.h"

using namespace DirectX;

//Global Variables 
HINSTANCE hInst = NULL;
HWND hWnd = NULL;
char windowTitle[100] = "Tutorial";
//DirectX variables 
D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* pD3DDevice = NULL;
ID3D11DeviceContext* pImmediateContext = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D11RenderTargetView* pBackBufferRTView = NULL;
UINT screenWidth, screenHeight;
//Background Colour
float rgba_clear_colour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//Input 
IDirectInput8* pDirectInput;
IDirectInputDevice8* pMouseDevice;
IDirectInputDevice8* pKeyboardDevice;
unsigned char keyboardStates[256];
DIMOUSESTATE mouseStates;

//Define vertex structure
struct POS_COL_TEX_NORM_VERTEX
{
    XMFLOAT3 pos;
    XMFLOAT4 col;
    XMFLOAT2 texture0;
    XMFLOAT3 normal;
};
//Triangle - screen coordiantes -1 to +1
POS_COL_TEX_NORM_VERTEX vertices[] =
{
    //Cube tutorial 
    //Back face
    {XMFLOAT3(-1.0f, 1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, 1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, 1.0f, 1.0f)   , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, 1.0f, 1.0f)   , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, 1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, -1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
    // front face
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    {XMFLOAT3(-1.0f, 1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    {XMFLOAT3(1.0f, -1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    // left face
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(-1.0f, -1.0f, 1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(-1.0f, 1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(-1.0f, -1.0f, 1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(-1.0f, 1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(-1.0f, 1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
    // right face
    {XMFLOAT3(1.0f, -1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(1.0f, -1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(1.0f, 1.0f, 1.0f)   , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(1.0f, -1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
    // bottom face
    {XMFLOAT3(1.0f, -1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
    {XMFLOAT3(1.0f, -1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
    {XMFLOAT3(1.0f, -1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
    {XMFLOAT3(-1.0f, -1.0f, 1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
    // top face
    {XMFLOAT3(1.0f, 1.0f, 1.0f)   , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
    {XMFLOAT3(-1.0f, 1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
    {XMFLOAT3(-1.0f, 1.0f, 1.0f)  , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
    {XMFLOAT3(1.0f, 1.0f, 1.0f)   , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
    {XMFLOAT3(-1.0f, 1.0f, -1.0f) , XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},


    //Cube (using triangle strips)
   /* {XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    {XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    {XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    {XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    {XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)}, */

    //Pyramid (using triangle strip)
    //{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    //{XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    //{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    //{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    //{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)}, 
    //{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
    //{XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    //{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},

};

//Model 
std::vector<Model*> pObject;

//Constant buffer
struct CONSTANT_BUFFER0
{
    XMMATRIX worldViewProjection;   //64 bytes (4x4x(4)) = 64bytes
    XMMATRIX worldView;   //64 bytes (4x4x(4)) = 64bytes
    XMVECTOR directionalLightVector; //16 bytes
    XMVECTOR directionalLightColour; //16 bytes
    XMVECTOR ambientLightColour;     //16 bytes   
    XMVECTOR pointLightPos;          //16 bytes
    XMVECTOR pointLightColour;       //16 bytes

    XMFLOAT3 pointLightAttenuation;  //12 bytes
    float range;                     //4 bytes
};  //Total size = 224 bytes 
ID3D11Buffer* pConstantBuffer;

//Matrices (transformations) cube
XMMATRIX rotation = XMMatrixRotationX(XMConvertToRadians(0.0f));
XMMATRIX translation = XMMatrixTranslation(0, 0, 5);
XMMATRIX scale = XMMatrixScaling(1, 1, 1);
XMMATRIX world_transf = scale * rotation * translation;
float rotx = 0, roty = 0, zTranslation = 5;
//Viewport 
Camera* camera;
const float cameraSpeed = 0.005f;
bool rightPressed;
float i = 0;

//Lights
AmbientLight* ambientLight;
DirectionalLight* directionalLight;
PointLight* pointLight;

//Skybox
ID3D11RasterizerState* pRasterSolid;
ID3D11RasterizerState* pRasterSkyBox;
ID3D11DepthStencilState* pDepthWriteSolid;
ID3D11DepthStencilState* pDepthWriteSkyBox;
ID3D11ShaderResourceView* skyboxTexture;

//Vertex buffer stuff
ID3D11Buffer* pVertexBuffer;
ID3D11VertexShader* pVertexShader;
ID3D11PixelShader* pPixelShader;
ID3D11InputLayout* pInputLayout;
//Z-Buffer
ID3D11DepthStencilView* pZBuffer;
//Texture and sampler
ID3D11ShaderResourceView* pTexture0;
ID3D11SamplerState* pSampler0;
//Text2D and blend states
Text2D* textUI;
ID3D11BlendState* pAlphaBlendEnable;
ID3D11BlendState* pAlphaBlendDisable;
//Rasterizer state
ID3D11RasterizerState* rastStateCullNone;
ID3D11RasterizerState* rastStateCullBack;

//Forward declaration
HRESULT InitWindow(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3D();
void ShutdownD3D();
void RenderFrame();
HRESULT InitGraphics();
void UpdateConstantBuffer(XMMATRIX _trans, XMMATRIX _scale, XMMATRIX _rot);
HRESULT InitInput();
void ReadInputStates();
bool IsKeyPressed(unsigned char _key);
bool IsRightClickPressed();
bool IsLeftClickPressed();
XMFLOAT2 GetMouseMovement();

//WinMain is the entry point of the program 
//Init everything and goes into a message processing loop
//Idle time is used to render the scene 
int WINAPI WinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, 
    _In_ int nCmdShow)
{
    App app; 

    return app.Loop();

    ////macro that simply stops compiler warnings about unused variables.
    //UNREFERENCED_PARAMETER(hPrevInstance);
    //UNREFERENCED_PARAMETER(lpCmdLine);

    //if (FAILED(InitWindow(hInstance, nCmdShow)))
    //{
    //    DXTRACE_MSG("Failed to create Window");
    //    return 0;
    //}

    ////Init input
    //if (FAILED(InitInput()))
    //{
    //    DXTRACE_MSG("Failed to create input device");
    //    return 0;
    //}

    ////Main message loop
    //MSG msg = { 0 };

    ////Direct3D init
    //if (FAILED(InitD3D()))
    //{
    //    DXTRACE_MSG("Failed to create D3D Device");
    //    return 0;
    //}

    //if (FAILED(InitGraphics()))
    //{
    //    DXTRACE_MSG("Failed to initialise graphics");
    //    return 0;
    //}

    //while (msg.message != WM_QUIT)
    //{
    //    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))   //Looks for messages
    //    {
    //        //TranslateMessage will post auxilliary WM_CHAR messages from key msgs
    //        //translates virtual-key messages into characters (WM_CHAR) (basically sends a WM_CHAR messages when pressing a key, so it can also be omitted if not using that message)
    //        TranslateMessage(&msg);		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage

    //        //Pass msg to the WndProc 
    //        //DefWindowProc doesn't know whether we want to quit the whole application or 
    //        //we just close the window, so when closing the app will still run
    //        DispatchMessage(&msg);		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
    //    }
    //    else
    //    {
    //        //Do something 
    //        RenderFrame();
    //    }
    //}

    ////Direct3D cleanup
    //ShutdownD3D();

    //return (int)msg.wParam;
}

////Register class and create window 
//HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
//{
//    //Give app the name you want 
//    char name[100] = "Hello World!";
//
//    //Register a window class first
//    WNDCLASSEX wcex = { 0 };    //init all elements to 0
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = WndProc; //Name of the function that processes all windows messages
//    wcex.hInstance = hInstance;
//    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);    //Needed for non-D3D apps (background clear colour)
//    wcex.lpszClassName = name;
//
//    if (!RegisterClassEx(&wcex)) return E_FAIL; //register the window class
//
//    //Create window (create an instance(s) of the window class)
//    hInst = hInstance;
//    RECT rc = { 0,0,1280,720 };
//    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);  //Adjust screen size without taking window borders into account)
//    screenWidth = rc.right - rc.left;
//    screenHeight = rc.bottom - rc.top;
//    hWnd = CreateWindow(
//        name,                   //Class name
//        windowTitle,            //title
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT,
//        CW_USEDEFAULT,
//        screenWidth,
//        screenHeight,
//        NULL,
//        NULL,
//        hInstance,              //Same value from WinMain
//        NULL
//    );
//
//    if (!hWnd)
//        return E_FAIL;
//
//    ShowWindow(hWnd, nCmdShow);
//
//    return S_OK;
//}
//
////Called every time the application receives a message
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    PAINTSTRUCT ps;
//    HDC hdc;
//
//    switch (message)
//    {
//    case WM_PAINT:  //Draws the window background.
//        hdc = BeginPaint(hWnd, &ps);
//        EndPaint(hWnd, &ps);
//        break;
//    case WM_DESTROY:    //Provides a WM_QUIT that will end the loop
//        PostQuitMessage(0);
//        break;
//    case WM_MOUSEWHEEL:
//    {
//        short delta = GET_WHEEL_DELTA_WPARAM(wParam);
//        if (delta > 0)
//        {
//            //Mouse Wheel Up
//            zTranslation++;
//        }
//        else
//        {
//            //Mouse Wheel Down
//            zTranslation--;
//        }
//        return 0;
//    }
//    break;
//    case WM_SIZE:
//        if (pSwapChain)
//        {
//            ID3D11RenderTargetView* nullViews[] = { nullptr };
//            pImmediateContext->OMSetRenderTargets(1, nullViews, 0);
//            //Release all outstanding buffers
//            pBackBufferRTView->Release();
//
//            HRESULT hr = S_OK;
//            //if 0, DXGI will use the width of the client area
//            hr = pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
//
//            if (FAILED(hr)) return hr;
//
//            //Get buffer and create a render-target-view
//            ID3D11Texture2D* pBackBufferTexture;
//            //Returns pointer to the back buffer 
//            hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
//            if (FAILED(hr)) return hr;
//
//            //Use back buffer texture pointer to create the renedr target view
//            hr = pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pBackBufferRTView);
//            if (FAILED(hr)) return hr;
//            //Clean up
//            pBackBufferTexture->Release();
//
//            //Clear the Z buffer
//            pImmediateContext->ClearDepthStencilView(pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//
//            RECT rc;
//            GetClientRect(hWnd, &rc);
//            screenWidth = rc.right - rc.left;
//            screenHeight = rc.bottom - rc.top;
//            stringstream a;
//            a << std::to_string(screenWidth) << " " << std::to_string(screenHeight) << "\n";
//            OutputDebugString(a.str().c_str());
//            camera->SetProjectionValues(90.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.01f, 1000.0f);
//
//            //Create a Z buffer texture 
//            D3D11_TEXTURE2D_DESC tex2DDesc;
//            ZeroMemory(&tex2DDesc, sizeof(tex2DDesc));
//
//            tex2DDesc.Width = screenWidth;
//            tex2DDesc.Height = screenHeight;
//            tex2DDesc.ArraySize = 1;
//            tex2DDesc.MipLevels = 1;
//            tex2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24 bits of each pixel 
//            tex2DDesc.SampleDesc.Count = 1;
//            tex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//            tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
//
//            ID3D11Texture2D* pZBufferTexture;
//            hr = pD3DDevice->CreateTexture2D(&tex2DDesc, NULL, &pZBufferTexture);
//
//            if (FAILED(hr)) return hr;
//
//            //Create the Z buffer
//            D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
//            ZeroMemory(&dsvDesc, sizeof(dsvDesc));
//
//            dsvDesc.Format = tex2DDesc.Format;
//            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//
//            pZBuffer->Release();
//            pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &pZBuffer);
//            pZBufferTexture->Release();
//
//            //Set the render target view 
//            pImmediateContext->OMSetRenderTargets(1, &pBackBufferRTView, pZBuffer);
//
//            //Set the viewport
//            D3D11_VIEWPORT viewport;
//            viewport.TopLeftX = 0;
//            viewport.TopLeftY = 0;
//            viewport.Width = (FLOAT)screenWidth;
//            viewport.Height = (FLOAT)screenHeight;
//            viewport.MinDepth = .0f;
//            viewport.MaxDepth = 1.0f;
//
//            pImmediateContext->RSSetViewports(1, &viewport);
//        }
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);    //Process any window messages
//    }
//    return 0;
//}
//
//
////Create D3D device and swap chain
//HRESULT InitD3D()
//{
//    HRESULT hr = S_OK;
//
//    RECT rc;
//    GetClientRect(hWnd, &rc);
//    UINT width = rc.right - rc.left;
//    UINT height = rc.bottom - rc.top;
//
//    UINT createDeviceFlags = 0;
//
//#ifdef _DEBUG   //Enables the devices debugging capabilities
//    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//    //Possible driver types
//    D3D_DRIVER_TYPE driverTypes[] =
//    {
//        D3D_DRIVER_TYPE_HARDWARE,   //Comment to test D3D 11.0 on hardware that doesn't support it 
//        D3D_DRIVER_TYPE_WARP,       //Comment to use reference device
//        D3D_DRIVER_TYPE_REFERENCE
//    };
//    UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//    D3D_FEATURE_LEVEL featureLevels[] =
//    {
//        D3D_FEATURE_LEVEL_11_0,
//        D3D_FEATURE_LEVEL_10_1,
//        D3D_FEATURE_LEVEL_10_0
//    };
//    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
//
//    //Swap chain descriptor
//    DXGI_SWAP_CHAIN_DESC sd;
//    ZeroMemory(&sd, sizeof(sd));    //Is the standard DirectX way of initialising structures. 
//    sd.BufferCount = 1;     //Number of back buffers in the swap chain
//    sd.BufferDesc.Width = width;    //width and height of the renderable area calculated earlier
//    sd.BufferDesc.Height = height;
//    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //Format of the buffers
//    sd.BufferDesc.RefreshRate.Numerator = 60;   //Refresh rate (in this case 60/1)
//    sd.BufferDesc.RefreshRate.Denominator = 1;
//    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   //Use the surface or resource as an output render target
//    sd.OutputWindow = hWnd;
//    sd.SampleDesc.Count = 1;    //No antialiasing
//    sd.SampleDesc.Quality = 0;
//    sd.Windowed = true;
//
//    //This loop goes through each of the specified devices and attempts to create an
//    //associated swap chain based on provided feature levels and the swap chain description
//    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//    {
//        driverType = driverTypes[driverTypeIndex];
//        hr = D3D11CreateDeviceAndSwapChain(
//            NULL,   //Graphics adapter (NULL == default)
//            driverType,     //Drivers to create
//            NULL,
//            createDeviceFlags,
//            featureLevels,
//            numFeatureLevels,
//            D3D11_SDK_VERSION,  //Always use this for DX apps
//            &sd,        //Poiter to the swap chain descriptor
//            &pSwapChain,    //pp to the swap chain object (used to access the swap chain)
//            &pD3DDevice,    //pp to the device object. represents the physical hardware
//            &featureLevel,  //Flag of the highest feature level that was found furing device creation
//            &pImmediateContext   //pp to the context object. represents the portion of the device that handles graphics rendering 
//        );
//
//        if (SUCCEEDED(hr))
//            break;
//    }
//
//    if (FAILED(hr))
//        return hr;
//
//    //Render targets 
//    //Get pointer to back buffer texture 
//    ID3D11Texture2D* pBackBufferTexture;
//    //Returns pointer to the back buffer 
//    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);
//    if (FAILED(hr)) return hr;
//
//    //Use back buffer texture pointer to create the renedr target view
//    hr = pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pBackBufferRTView);
//    if (FAILED(hr)) return hr;
//    //Clean up
//    pBackBufferTexture->Release();
//
//
//    //Create a Z buffer texture 
//    D3D11_TEXTURE2D_DESC tex2DDesc;
//    ZeroMemory(&tex2DDesc, sizeof(tex2DDesc));
//
//    tex2DDesc.Width = width;
//    tex2DDesc.Height = height;
//    tex2DDesc.ArraySize = 1;
//    tex2DDesc.MipLevels = 1;
//    tex2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24 bits of each pixel 
//    tex2DDesc.SampleDesc.Count = sd.SampleDesc.Count;
//    tex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//    tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
//
//    ID3D11Texture2D* pZBufferTexture;
//    hr = pD3DDevice->CreateTexture2D(&tex2DDesc, NULL, &pZBufferTexture);
//
//    if (FAILED(hr)) return hr;
//
//    //Blend states
//    D3D11_BLEND_DESC bsdesc;
//    bsdesc.RenderTarget[0].BlendEnable = TRUE;
//    bsdesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//    bsdesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
//    bsdesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//    bsdesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
//    bsdesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
//    bsdesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
//    bsdesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//    bsdesc.IndependentBlendEnable = FALSE;
//    bsdesc.AlphaToCoverageEnable = FALSE;
//    pD3DDevice->CreateBlendState(&bsdesc, &pAlphaBlendEnable);
//
//    //BackFace culling states (enable and disable)
//    D3D11_RASTERIZER_DESC rdesc;
//    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
//    rdesc.FillMode = D3D11_FILL_SOLID;
//    rdesc.CullMode = D3D11_CULL_NONE;
//    rdesc.FrontCounterClockwise = false;
//    rdesc.DepthBias = false;
//    rdesc.DepthBiasClamp = 0;
//    rdesc.SlopeScaledDepthBias = 0;
//    rdesc.DepthClipEnable = true;
//    rdesc.MultisampleEnable = true;
//    rdesc.AntialiasedLineEnable = true;
//    hr = pD3DDevice->CreateRasterizerState(&rdesc, &rastStateCullNone);
//
//    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
//    rdesc.FillMode = D3D11_FILL_SOLID;
//    rdesc.CullMode = D3D11_CULL_BACK;
//    rdesc.FrontCounterClockwise = false;
//    rdesc.DepthBias = false;
//    rdesc.DepthBiasClamp = 0;
//    rdesc.SlopeScaledDepthBias = 0;
//    rdesc.DepthClipEnable = true;
//    rdesc.MultisampleEnable = false;
//    rdesc.AntialiasedLineEnable = false;
//    hr = pD3DDevice->CreateRasterizerState(&rdesc, &rastStateCullBack);
//
//    //SkyBox stuff
//    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
//    rdesc.FillMode = D3D11_FILL_SOLID;
//    rdesc.CullMode = D3D11_CULL_BACK;
//    hr = pD3DDevice->CreateRasterizerState(&rdesc, &pRasterSolid);
//    rdesc.FillMode = D3D11_FILL_SOLID;
//    rdesc.CullMode = D3D11_CULL_FRONT;
//    hr = pD3DDevice->CreateRasterizerState(&rdesc, &pRasterSkyBox);
//
//    D3D11_DEPTH_STENCIL_DESC dpthStDesc;
//    ZeroMemory(&dpthStDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
//    dpthStDesc.DepthEnable = true;
//    dpthStDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//    dpthStDesc.DepthFunc = D3D11_COMPARISON_LESS;
//    hr = pD3DDevice->CreateDepthStencilState(&dpthStDesc, &pDepthWriteSolid);
//    dpthStDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//    hr = pD3DDevice->CreateDepthStencilState(&dpthStDesc, &pDepthWriteSkyBox);
//
//    //Create the Z buffer
//    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
//    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
//
//    dsvDesc.Format = tex2DDesc.Format;
//    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//
//    pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &pZBuffer);
//    pZBufferTexture->Release();
//
//    //Set the render target view 
//    pImmediateContext->OMSetRenderTargets(1, &pBackBufferRTView, pZBuffer);
//
//    //Set the viewport
//    D3D11_VIEWPORT viewport;
//    viewport.TopLeftX = 0;
//    viewport.TopLeftY = 0;
//    viewport.Width = (FLOAT)width;
//    viewport.Height = (FLOAT)height;
//    viewport.MinDepth = .0f;
//    viewport.MaxDepth = 1.0f;
//
//    pImmediateContext->RSSetViewports(1, &viewport);
//
//    return S_OK;
//}
//
//void ShutdownD3D()
//{
//    //Shaders clean up (before system otherwise you'll release 
//    //resources that were used to create these objects)
//    if (textUI != nullptr)
//    {
//        delete textUI;
//        textUI = nullptr;
//    }
//    if (camera != nullptr)
//    {
//        delete camera;
//        camera = nullptr;
//    }
//    for (int i = 0; i < pObject.size(); i++)
//    {
//        if (pObject[i] != nullptr)
//        {
//            delete pObject[i];
//            pObject[i] = nullptr;
//        }
//    }
//    if (ambientLight)
//    {
//        delete ambientLight;
//        ambientLight = nullptr;
//    }
//    if (directionalLight)
//    {
//        delete directionalLight;
//        directionalLight = nullptr;
//    }
//    if (pointLight)
//    {
//        delete pointLight;
//        pointLight = nullptr;
//    }
//    if (pKeyboardDevice)
//    {
//        pKeyboardDevice->Unacquire();
//        pKeyboardDevice->Release();
//    }
//    if (pMouseDevice != nullptr)
//    {
//        pMouseDevice->Unacquire();
//        pMouseDevice->Release();
//    }
//    if (pRasterSolid != nullptr) pRasterSolid->Release();
//    if (pRasterSkyBox != nullptr) pRasterSkyBox->Release();
//    if (pDepthWriteSolid != nullptr) pDepthWriteSolid->Release();
//    if (pDepthWriteSkyBox != nullptr) pDepthWriteSkyBox->Release();
//    if (pDirectInput) pDirectInput->Release();
//    if (pVertexBuffer) pVertexBuffer->Release();
//    if (pInputLayout) pInputLayout->Release();
//    if (pConstantBuffer) pConstantBuffer->Release();
//    if (pVertexShader) pVertexShader->Release();
//    if (pPixelShader) pPixelShader->Release();
//    if (pBackBufferRTView) pBackBufferRTView->Release();
//    if (pZBuffer) pZBuffer->Release();
//    if (pTexture0) pTexture0->Release();
//    if (pSampler0) pSampler0->Release();
//    if (pAlphaBlendEnable) pAlphaBlendEnable->Release();
//    if (pAlphaBlendDisable) pAlphaBlendDisable->Release();
//    if (rastStateCullBack) rastStateCullBack->Release();
//    if (rastStateCullNone) rastStateCullNone->Release();
//
//    //System clean up
//    if (pSwapChain) pSwapChain->Release();
//    if (pImmediateContext) pImmediateContext->Release();
//    if (pD3DDevice) pD3DDevice->Release();
//}
//
//void RenderFrame()
//{
//    //Read input
//    ReadInputStates();
//
//    if (IsKeyPressed(DIK_ESCAPE))DestroyWindow(hWnd);
//
//    //Move Camera
//    if (IsKeyPressed(DIK_W))
//        camera->AdjustPosition(camera->GetForwardVector() * cameraSpeed);
//    if (IsKeyPressed(DIK_A))
//        camera->AdjustPosition(camera->GetLeftVector() * cameraSpeed);
//    if (IsKeyPressed(DIK_S))
//        camera->AdjustPosition(camera->GetBackwardVector() * cameraSpeed);
//    if (IsKeyPressed(DIK_D))
//        camera->AdjustPosition(camera->GetRightVector() * cameraSpeed);
//    if (IsKeyPressed(DIK_SPACE))
//        camera->AdjustPosition(0.0f, cameraSpeed, 0.0f);
//    if (IsKeyPressed(DIK_Z))
//        camera->AdjustPosition(0.0f, -cameraSpeed, 0.0f);
//    //Camera rotation
//    if (IsRightClickPressed())
//    {
//        XMFLOAT2 movement = GetMouseMovement();
//        camera->AdjustRotation(0.01f * movement.x, 0.01f * movement.y, 0.0f);
//    }
//
//    //light input
//    if (IsKeyPressed(DIK_LEFT))
//        pointLight->Translate(-0.05f, 0.0f, 0.0f);
//    if (IsKeyPressed(DIK_RIGHT))
//        pointLight->Translate(0.05f, 0.0f, 0.0f);
//    if (IsKeyPressed(DIK_UP))
//        pointLight->Translate(0.0f, 0.0f, 0.05f);
//    if (IsKeyPressed(DIK_DOWN))
//        pointLight->Translate(0.0f, 0.0f, -0.05f);
//
//    //Clear back buffer
//    pImmediateContext->ClearRenderTargetView(pBackBufferRTView, rgba_clear_colour);
//
//    //Clear the Z buffer
//    pImmediateContext->ClearDepthStencilView(pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//
//    //Render here
//    UINT stride = sizeof(POS_COL_TEX_NORM_VERTEX);
//    UINT offset = 0;
//    //Set vertex buffer
//    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
//    //Select which primitive type to use 
//    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    //Copying the vertices into the buffer 
//    D3D11_MAPPED_SUBRESOURCE ms;
//    //Lock the buffer to allow writing (allows CPU access)
//    pImmediateContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//
//    //Copy the data into the vertex buffer
//    memcpy(ms.pData, vertices, sizeof(vertices));
//
//    //Unlock the buffer (tells Direct3D that the copy has finished 
//    //and that is free to use the buffer again)
//    pImmediateContext->Unmap(pVertexBuffer, NULL);
//
//    pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
//
//    //Set shaders every frame
//    pImmediateContext->VSSetShader(pVertexShader, 0, 0);
//    pImmediateContext->PSSetShader(pPixelShader, 0, 0);
//    //Set input layout every frame
//    pImmediateContext->IASetInputLayout(pInputLayout);
//
//    //Use sample states
//    pImmediateContext->PSSetSamplers(0, 1, &pSampler0);
//    pImmediateContext->PSSetShaderResources(0, 1, &pTexture0);
//
//    //Set raster state to cullBack
//    pImmediateContext->RSSetState(rastStateCullBack);
//
//
//    //CBuff Update and set active
//    i += 0.01f;
//    if (i > 360.0f)
//        i = 0.0f;
//    directionalLight->Rotate(0.001f, 0.0f, 0.0f);
//    rotation = XMMatrixRotationY(XMConvertToRadians(i));
//
//    //CBuff Update and set active and Draw pointLight 
//    XMMATRIX lightRot = XMMatrixRotationRollPitchYawFromVector(pointLight->GetRotationVector());
//    XMMATRIX lightPos = XMMatrixTranslationFromVector(pointLight->GetPositionVector());
//    XMMATRIX lightScale = XMMatrixScaling(0.05f, 0.05f, 0.05f);// XMMatrixScaling(XMVectorGetX(pointLightScale), XMVectorGetY(pointLightScale), XMVectorGetZ(pointLightScale));
//    UpdateConstantBuffer(lightPos, lightScale, lightRot);
//    pImmediateContext->Draw(sizeof(vertices) / sizeof(vertices[0]), 0);  //Vertices to draw, where to start (0) 
//
//    //Draw Skybox
//    pObject[2]->UpdateConstantBf(camera->GetViewMatrix(), camera->GetProjetionMatrix());
//    pObject[2]->SetPosition(camera->GetPositionVector());
//    pImmediateContext->RSSetState(pRasterSkyBox);
//    pImmediateContext->OMSetDepthStencilState(pDepthWriteSkyBox, 0);
//    pObject[2]->Draw();
//    pImmediateContext->OMSetDepthStencilState(pDepthWriteSolid, 0);
//    pImmediateContext->RSSetState(pRasterSolid);
//    //Set blend mode and Render text
//    pImmediateContext->OMSetBlendState(pAlphaBlendEnable, 0, 0xffffffff);
//    //Draw model (follow)
//    pObject[0]->LookAt_XYZ(pObject[1]->GetPosition().x, pObject[1]->GetPosition().y, pObject[1]->GetPosition().z);
//    pObject[0]->MoveForwardXY(0.0001f);
//    pObject[0]->UpdateConstantBf(camera->GetViewMatrix(), camera->GetProjetionMatrix(), ambientLight, directionalLight, pointLight);
//    pObject[0]->Draw();
//    if (pObject[0]->CheckCollision(pObject[1]))
//        pObject[0]->MoveBackwardsXY(0.0001f);
//    //Draw second model 
//    pObject[1]->UpdateConstantBf(camera->GetViewMatrix(), camera->GetProjetionMatrix(), ambientLight, directionalLight, pointLight);
//    pObject[1]->LookAt_XYZ(pObject[0]->GetPosition().x, pObject[0]->GetPosition().y, pObject[0]->GetPosition().z);
//    pObject[1]->Draw();
//
//    //Set raster state to cullNone
//    pImmediateContext->RSSetState(rastStateCullNone);
//    textUI->AddText("Hello world!", -1.0f, .5f, 0.05f);
//    textUI->RenderText();
//    pImmediateContext->OMSetBlendState(pAlphaBlendDisable, 0, 0xffffffff);
//
//    //Display what has just been rendered
//    pSwapChain->Present(0, 0);
//}
//
//HRESULT InitGraphics()
//{
//    HRESULT hr = S_OK;
//
//    //Set up and create vertex buffer 
//    D3D11_BUFFER_DESC bufferDesc;
//    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
//    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //Used by CPU and GPU (both allowed)
//    bufferDesc.ByteWidth = (sizeof(vertices) / sizeof(vertices[0])) * sizeof(POS_COL_TEX_NORM_VERTEX);  //Total size of the buffer
//    bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;    //Tells DX that this is a vertex buffer
//    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //Allows CPU access
//    hr = pD3DDevice->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);
//    if (FAILED(hr)) return hr;
//
//    //Set up and create constant buffer
//    D3D11_BUFFER_DESC cBufDesc;
//    ZeroMemory(&cBufDesc, sizeof(cBufDesc));
//    cBufDesc.Usage = D3D11_USAGE_DEFAULT;   //can use UpdateSubresource() to update
//    cBufDesc.ByteWidth = 224;    //MUST be a multiple of 16, calculate from CB struct
//    cBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    //Use as a constant buffer
//    hr = pD3DDevice->CreateBuffer(&cBufDesc, NULL, &pConstantBuffer);
//    if (FAILED(hr)) return hr;
//
//    //Copying the vertices into the buffer 
//    D3D11_MAPPED_SUBRESOURCE ms;
//
//    //Lock the buffer to allow writing (allows CPU access)
//    pImmediateContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//
//    //Copy the data into the vertex buffer
//    memcpy(ms.pData, vertices, sizeof(vertices));
//
//    //Unlock the buffer (tells Direct3D that the copy has finished 
//    //and that is free to use the buffer again)
//    pImmediateContext->Unmap(pVertexBuffer, NULL);
//
//    //Load and compile the pixel and vertex shaders - use vs_5_0 to target hardware only
//    ID3DBlob* VS, * PS, * error;
//    hr = D3DX11CompileFromFile("VertexShader.hlsl", 0, 0, "main", "vs_4_0", 0, 0, 0, &VS, &error, 0);
//    if (error != 0)
//    {
//        OutputDebugString((char*)error->GetBufferPointer());
//        error->Release();
//        if (FAILED(hr)) return hr;
//    }
//    hr = D3DX11CompileFromFile("PixelShader.hlsl", 0, 0, "main", "ps_4_0", 0, 0, 0, &PS, &error, 0);
//    if (error != 0)
//    {
//        OutputDebugString((char*)error->GetBufferPointer());
//        error->Release();
//        if (FAILED(hr)) return hr;
//    } 
//
//    //Create shader objects
//    hr = pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVertexShader);
//    if (FAILED(hr)) return hr;
//
//    hr = pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPixelShader);
//    if (FAILED(hr)) return hr;
//
//    //Create Texture 
//    hr = D3DX11CreateShaderResourceViewFromFile(pD3DDevice, "Assets/BoxTexture.bmp", NULL, NULL, &pTexture0, NULL);
//    if (FAILED(hr)) return hr; 
//    //Create Sampler
//    D3D11_SAMPLER_DESC samplerDesc;
//    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
//    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//    pD3DDevice->CreateSamplerState(&samplerDesc, &pSampler0);
//
//    //Set the shader objects as active + constant buffer 
//    pImmediateContext->VSSetShader(pVertexShader, 0, 0);
//    pImmediateContext->PSSetShader(pPixelShader, 0, 0);
//    pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
//
//    //Create and set the imput layout object 
//    D3D11_INPUT_ELEMENT_DESC iedesc[] =
//    {
//        //Be very careful setting the correct dxgi format and D3D version 
//        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//    };
//
//    hr = pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &pInputLayout);
//    if (FAILED(hr)) return hr;
//
//    //Set input layout
//    pImmediateContext->IASetInputLayout(pInputLayout);
//
//    //Camera 
//    camera = new Camera();
//    camera->SetPosition(0.0f, 0.0f, -2.0f);
//    camera->SetProjectionValues(90.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.01f, 1000.0f);
//
//    //Model
//    //pObject.push_back(new Model(pD3DDevice, pImmediateContext));
//    //pObject[0]->LoadObjModel("Assets/Sphere.obj", "ReflectVS", "ReflectPS");
//    //pObject[0]->SetTexture("Assets/City.dds");
//    //pObject[0]->Scale(0.2f, 0.2f, 0.2f);
//
//    //pObject.push_back(new Model(pD3DDevice, pImmediateContext));
//    //pObject[1]->LoadObjModel("Assets/PointySphere.obj");
//    //pObject[1]->SetTexture("Assets/Helm_BaseColor.png");
//    //pObject[1]->SetPosition(0.0f, 0.0f, 8.0f);
//    //pObject[1]->Scale(0.5f, 0.5f, 0.5f);
//
//    ////Skybox
//    //pObject.push_back(new Model(pD3DDevice, pImmediateContext));
//    //pObject[2]->LoadObjModel("Assets/cube.obj", "SkyBoxVS", "SkyBoxPS");
//    //pObject[2]->SetTexture("Assets/City.dds");
//    //pObject[2]->Scale(3.0f, 3.0f, 3.0f);
//
//    //Lights
//    ambientLight = new AmbientLight();
//    ambientLight->SetColour(0.3f, 0.3f, 0.3f, 1.0f);
//    directionalLight = new DirectionalLight();
//    directionalLight->SetColour(0.98f, 0.84f, 0.11f, 0.0f);
//    directionalLight->SetDirection(0.5f, 0.5f, -1.0f);
//    pointLight = new PointLight();
//    pointLight->SetColour(0.0f, 10.0f, 0.0f, 1.0f);
//    pointLight->SetAttenuation(XMFLOAT3(0.0f, 1.0f, 0.0f));
//    pointLight->SetRange(10.0f);
//
//    //Text2D
//    textUI = new Text2D("Assets/font1.png", pD3DDevice, pImmediateContext);
//
//    return S_OK;
//}
//
//void UpdateConstantBuffer(XMMATRIX _trans, XMMATRIX _scale, XMMATRIX _rot)
//{
//    CONSTANT_BUFFER0 cb0;
//    world_transf = _scale * _rot * _trans;
//
//    //Ambient light
//    cb0.ambientLightColour = ambientLight->GetColour();
//    //Directional Light
//    cb0.directionalLightColour = directionalLight->GetColour();
//    cb0.directionalLightVector = directionalLight->GetDirection(world_transf);
//    //point light and Attenuation
//    cb0.pointLightPos = pointLight->GetInversePos(world_transf);
//    cb0.pointLightColour = pointLight->GetColour();
//    cb0.pointLightAttenuation = pointLight->GetAttenuation(); //constant falloff factor
//    cb0.range = pointLight->GetRange();
//
//    cb0.worldViewProjection = world_transf * camera->GetViewMatrix() * camera->GetProjetionMatrix();
//
//    //upload the new values for the constant buffer 
//    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, 0, &cb0, 0, 0);
//}
//
//HRESULT InitInput()
//{
//    HRESULT hr = S_OK;
//
//    //Create DirectInput object 
//    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL);
//    if (FAILED(hr)) return hr;
//
//#pragma region Keyboard;
//    ZeroMemory(keyboardStates, sizeof(keyboardStates));
//
//    //Create device 
//    hr = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboardDevice, NULL);
//    if (FAILED(hr)) return hr;
//
//    //Tell DirectInput how to interpret the device (keyboard)
//    hr = pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
//    if (FAILED(hr)) return hr;
//
//    //Tell device how ti interact with other Apps  (DISCL: only if window is active, and app can share the device with other apps)
//    hr = pKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//    if (FAILED(hr)) return hr;
//
//    //Ask the device to start providing input for the App
//    hr = pKeyboardDevice->Acquire();
//    if (FAILED(hr)) return hr;
//#pragma endregion
//
//#pragma region Mouse 
//    //Create device 
//    hr = pDirectInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);
//    if (FAILED(hr)) return hr;
//
//    //Tell DirectInput how to interpret the device (keyboard)
//    hr = pMouseDevice->SetDataFormat(&c_dfDIMouse);
//    if (FAILED(hr)) return hr;
//
//    //Tell device how ti interact with other Apps  (DISCL: only if window is active, and app can share the device with other apps)
//    hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
//    if (FAILED(hr)) return hr;
//
//    //Ask the device to start providing input for the App
//    hr = pMouseDevice->Acquire();
//    if (FAILED(hr)) return hr;
//#pragma endregion 
//
//    return S_OK;
//}
//
//void ReadInputStates()
//{
//    HRESULT hr;
//
//#pragma region Keyboard;
//    hr = pKeyboardDevice->GetDeviceState(sizeof(keyboardStates), (LPVOID)&keyboardStates);
//
//    //Sometimes the app window loses focus, so reaquire the device;
//    if (FAILED(hr))
//    {
//        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
//        {
//            pKeyboardDevice->Acquire();
//        }
//    }
//#pragma endregion
//
//#pragma region Mouse 
//    hr = pMouseDevice->GetDeviceState(sizeof(mouseStates), (LPVOID)&mouseStates);
//
//    //Sometimes the app window loses focus, so reaquire the device;
//    if (FAILED(hr))
//    {
//        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
//        {
//            pMouseDevice->Acquire();
//        }
//    }
//#pragma endregion
//}
//
//bool IsKeyPressed(unsigned char _key)
//{
//    return keyboardStates[_key];
//}
//
//bool IsRightClickPressed()
//{
//    return mouseStates.rgbButtons[1];
//}
//
//bool IsLeftClickPressed()
//{
//    return mouseStates.rgbButtons[0];
//}
//
//XMFLOAT2 GetMouseMovement()
//{
//    return XMFLOAT2{ (float)mouseStates.lY, (float)mouseStates.lX };
//}
