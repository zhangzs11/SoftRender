#pragma once
#include<iostream>
#include<fstream>
#include<windows.h>
#include<vector>
#include<string>
#include"Math.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

typedef struct Mesh
{
	std::vector<VECTOR4> positionBuffer;
	std::vector<VECTOR4> normalBuffer;
	std::vector<VECTOR4> uvBuffer;
	std::vector<std::vector<VECTOR4>> indexBuffer;
}Mesh;
typedef struct SimpleVertex
{
	VECTOR4 Pos;
	COLOR Color;
	VECTOR4 uv;
	VECTOR4 Normal;
	VECTOR4 PosWorld;
	VECTOR4 tangent;
	VECTOR4 bitangent;
	MATRIX TBN;
}SimpleVertex;
typedef struct Triangle
{
	SimpleVertex vertex[3];
	Triangle* next = NULL;
}Triangle;
typedef struct ConstantBuffer
{
	MATRIX World;
	MATRIX View;
	MATRIX Projection;
	VECTOR4* eye = new VECTOR4(10.0f, 4.0f, -20.0f, 0.0f);
	VECTOR4* At = new VECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
	VECTOR4* Up = new VECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
	VECTOR4 cameraZaxis;
	VECTOR4 cameraXaxis;
	VECTOR4 cameraYaxis;

	float ka;
	float kd;
	float ks;
	float a;
	VECTOR4 lightPos;
	COLOR id;
	COLOR is;
	COLOR ia;
}ConstantBuffer;
typedef struct Texture2D
{
public:
	COLOR data[1024][768] = { COLOR(0, 0, 0, 0) };
	void Release();
	int width = 200;
	int height = 200;

	void LoadTexture(LPCWSTR path, int width, int height);
	int readTGA(std::string filename);
}Texture2D;
struct TGAHeader
{
	uint8_t idLength;
	uint8_t colormapType;
	uint8_t imageType;
	uint16_t colormapOrigin;
	uint16_t colormapLength;
	uint8_t colormapDepth;
	uint16_t xOrigin;
	uint16_t yOrigin;
	uint16_t width;
	uint16_t height;
	uint8_t bitsPerPixel;
	uint8_t imageDescriptor;
};
typedef struct BUFFER_DESC
{
public:
	UINT ByteWidth;
}BUFFER_DESC;
typedef struct SUBRESOURCE_DATA
{
public:
	void* pSyMem;
}SUBRESOURCE_DATA;
typedef struct Buffer
{
public:
	BUFFER_DESC desc;
	SUBRESOURCE_DATA data;
}Buffer;
typedef struct RenderTargetView
{
public:
	float Width = 0;
	float Height = 0;
	float MinDepth = 0;
	float MaxDepth = 0;
	float TopLeftX = 0;
	float TopLeftY = 0;

	Texture2D* texture2d;
	void Release();
}RenderTargetView;
typedef struct DepthStencilView
{
public:
	Texture2D* texture2d;
	void Release();
}DepthStencilView;
typedef struct VIEWPORT
{
public:
	float Width = 0;
	float Height = 0;
	float MinDepth = 0;
	float MaxDepth = 0;
	float TopLeftX = 0;
	float TopLeftY = 0;

	void Release();
}VIEWPORT;

typedef struct Device
{
public:
	HRESULT CreateRenderTargetView(Texture2D* pTexture, RenderTargetView** ppRTView);
	HRESULT CreateDepthstencView(Texture2D* pTexture, DepthStencilView** ppDSView);
	//HRESULT CreateInputLayout();
	HRESULT CreateBuffer(const BUFFER_DESC* pDesc, SUBRESOURCE_DATA* pInitialData, Buffer** ppBuffer);
	HRESULT CreateViewPort(VIEWPORT** vp, int* width, int* height);
	HRESULT CreateMesh(Mesh** pmesh);
	void Release();
}Device;
typedef struct DeviceContext
{
public:
	void SetRenderTarget(UINT NumViews, RenderTargetView* ppRenderTargetViews, DepthStencilView* pDepthStencilView);
	void SetViewports(UINT numviewports, VIEWPORT* pViewports);
	void SetVertexBuffer(UINT Numbuffers, Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
	void SetIndexBuffer(Buffer* pIndexBuffer, UINT Offset);
	void SetConstantBuffer(UINT StartSlot, UINT NumBuffers, Buffer* const* ppConstantBuffers);
	void SetPrimitiveTopology(UINT primitive);
	void SetMesh(Mesh* pMesh);

	void Draw();
	void DarwIndexed(UINT IndexCount, UINT startIndexLocation, UINT BaseVertexLocation);
	void DrawMesh();
	void DrawLight();
	void fillBottomFlatTriangle(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3);
	void fillTopFlatTriangle(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3);
	void drawTriangle(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3);
	void ClearRenderTargetView();
	void ClearDepthView();
	void Clearnormaltexture();
	void ClearState();
	void Release();

	SimpleVertex VertexShader(SimpleVertex input);
	COLOR PixelShader(VECTOR4 texcoord, VECTOR4 normal, VECTOR4 pos, MATRIX TBN);

	RenderTargetView* rendertargetview;
	DepthStencilView* depthstencilview;
	VIEWPORT* viewport;
	Buffer* vertexbuffer;
	Buffer* indexbuffer;
	Buffer* constantbuffer;
	UINT primative;
	Texture2D texturesource;
	Texture2D normaltexturesource;
	Mesh* meshsource;//读取的模型资源

	int rendermodel=1;
}DeviceContext;

typedef struct SWAP_CHAIN_DESC
{
public:
	UINT BufferCount = 1;
	UINT Width = 0;
	UINT Height = 0;
	UINT RefreshRate = 60;
	HWND OutputWindow;//作显示的窗口句柄
}SWAP_CHAIN_DESC;
typedef struct SwapChain
{
public:
	SWAP_CHAIN_DESC desc;
	HRESULT GetBuffer(UINT Buffer, Texture2D** ppSurface);
	void Present();
	void Release();

	Texture2D* surface;
}SwapChain;

typedef struct Renderer
{
public:
	HRESULT CreateDeviceAndSwapChain(SWAP_CHAIN_DESC* pSwapChainDesc, SwapChain** ppSwapChain, Device** ppDevice, DeviceContext** ppContext);

	SwapChain* swapchain;
	Device* device;
	DeviceContext* devicecontext;
}Renderer;
void DDA_Line(int x1, int x2, int y1, int y2, COLOR color1, COLOR color2, DeviceContext* devicecontext);//DDA画线算法
bool isPointInTrangle(VECTOR4 point, VECTOR4 a, VECTOR4 b, VECTOR4 c);//叉乘判断点是否在三角形内部
SimpleVertex wviewport(SimpleVertex input);
bool ClipSpaceCull(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3, float Near, float Far);//是否三角形至少有一部分在屏幕内
bool FaceCull(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3);//是否背面剔除
bool Inside(VECTOR4 line, VECTOR4 p);//判断点是否在平面内，通过看点和平面的距离d，d>0则在平面法向量所指的区域内
SimpleVertex Intersect(SimpleVertex v1, SimpleVertex v2, VECTOR4 line);//求点与线段的交点，通过端点插值
bool AllVertexInside(Triangle input, float Near, float Far);
std::vector<Triangle> SutherlandHodgeman(std::vector<Triangle> input, float Near, float Far);//输入当前帧的所有三角形列表，输出裁剪后的三角形列表
void StringSplit(std::string s, char splitchar, std::vector<std::string>& vec);
void ReadObjFile(std::string path, Mesh* obj);