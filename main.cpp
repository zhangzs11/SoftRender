#include"Graphics.h"
#include<iostream>
#include<windows.h>

Renderer renderer;
BYTE buffer[SCREEN_WIDTH * SCREEN_HEIGHT * 3];//һ������ռ���ֽ�
HDC screen_hdc;
HWND screen_hwnd;
HDC hCompatibleDC;//����HDC
HBITMAP hCompatibleBitmap;//����BITMAP
HBITMAP hOldBitmap;//�ɵ�BITMAP
BITMAPINFO binfo;//BITMAPINFO�ṹ��

HDC hdc;
HINSTANCE g_hInst = NULL;
HWND g_hwnd = NULL;
Device* g_pd3dDevice = NULL;
DeviceContext* g_pImmediateContext = NULL;
SwapChain* g_pSwapChain = NULL;
RenderTargetView* g_pRenderTargetView = NULL;
DepthStencilView* g_pDepthStencilView = NULL;
Buffer* g_pVertexsBuffer = NULL;
Buffer* g_pIndexBuffer = NULL;
Buffer* g_pConstantBuffer = NULL;
Mesh* g_mesh = NULL;

MATRIX world;
MATRIX view;
MATRIX projection;
VECTOR3* eye = new VECTOR3(0.0f, 0.0f, -20.0f);
VECTOR3* At = new VECTOR3(0.0f, 1.0f, 0.0f);
VECTOR3* Up = new VECTOR3(0.0f, 1.0f, 0.0f);
VECTOR3 cameraZaxis;
VECTOR3 cameraXaxis;
VECTOR3 cameraYaxis;

UINT indices[] =
{
	0, 1, 2,
};
SimpleVertex vertices[] =
{
	{VECTOR4(2.0f, 3.0f, 3.0f, 1.0f), COLOR(0, 255, 0, 1), VECTOR4(0, 0, 0, 0),VECTOR4(0, 0, 0, 0) },
	{VECTOR4(2.0f, 3.0f, 3.0f, 1.0f), COLOR(0, 255, 0, 1), VECTOR4(0, 0, 0, 0),VECTOR4(0, 0, 0, 0) },
	{VECTOR4(2.0f, 3.0f, 3.0f, 1.0f), COLOR(0, 255, 0, 1), VECTOR4(0, 0, 0, 0),VECTOR4(0, 0, 0, 0) },
};
ConstantBuffer cb;

//��������
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool initdevicefinished = false;
void Render(int indexnum);
void resetConstantBuffer(ConstantBuffer* cb);
void DrawFPS(HDC hdc);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	bool devicefinished = false;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	AllocConsole();//create console
	SetConsoleTitle(L"DebugConsole");
	FILE* tempFile = nullptr;
	freopen_s(&tempFile, "conin", "r+t", stdin);
	freopen_s(&tempFile, "conout$", "r+t", stdout);
	std::cout << "main window!" << std::endl;
	if (FAILED(InitWindow(hInstance, nCmdShow))) {
		return 0;
	}
	if (FAILED(InitDevice())) {
		CleanupDevice();
		return 0;
	}
	devicefinished = true;
	//���BITMAPINFO�ṹ��
	ZeroMemory(&binfo, sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount = 24;//ÿ�����ض���λ
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSizeImage = 0;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth = SCREEN_WIDTH;
	//��ȡ��ĻHDC
	screen_hwnd = g_hwnd;
	screen_hdc = GetDC(screen_hwnd);
	//��ȡ���ݵ�HDC�ͼ���Bitmap�� ����Bitmapѡ�����HDC(ÿ��HDC�ڴ�ÿʱÿ�̽���ѡ��һ��GDI��Դ�� GDI��ԴҪѡ��HDC���ܽ��м��)
	hCompatibleDC = CreateCompatibleDC(screen_hdc);
	hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
	hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);

	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Render(0);
			//DrawFPS(screen_hdc);
		}
	}
	CleanupDevice();
	return (int)msg.wParam;
}
//��������
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	//ע�ᴰ��
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)107);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)107);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;
	//��������
	g_hInst = hInstance;
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hwnd = CreateWindow(L"TutorialWindowClass", L"SoftRender", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hwnd)
		return E_FAIL;
	ShowWindow(g_hwnd, nCmdShow);
	return S_OK;
}
HRESULT InitDevice()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(g_hwnd, &rc);
	UINT width = 1024;
	UINT height = 768;
	//����������
	SWAP_CHAIN_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.Width = width;
	sd.Height = height;
	sd.RefreshRate = 60;
	sd.OutputWindow = g_hwnd;
	hr = renderer.CreateDeviceAndSwapChain(&sd, &g_pSwapChain, &g_pd3dDevice, &g_pImmediateContext);
	if (FAILED(hr))
		return hr;
	//������Ⱦ����
	Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, &pBackBuffer);
	if (FAILED(hr))
		return hr;
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;
	//������Ȼ�������
	Texture2D* pDepthBuffer = NULL;
	hr = g_pd3dDevice->CreateDepthstencView(pDepthBuffer, &g_pDepthStencilView);
	pDepthBuffer->Release();
	g_pImmediateContext->SetRenderTarget(1, g_pRenderTargetView, g_pDepthStencilView);
	//�������㻺����
	BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(SimpleVertex) * 8;
	SUBRESOURCE_DATA InitData;
	SecureZeroMemory(&InitData, sizeof(InitData));
	InitData.pSyMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexsBuffer);
	if (FAILED(hr))
		return hr;
	//���ö��㻺����
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->SetVertexBuffer(1, &g_pVertexsBuffer, &stride, &offset);
	//��������������
	bd.ByteWidth = sizeof(WORD) * 8;
	InitData.pSyMem = indices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;
	//��������������
	g_pImmediateContext->SetIndexBuffer(g_pIndexBuffer, 0);
	//����ͼԪ
	g_pImmediateContext->SetPrimitiveTopology(4);
	//��������������
	bd.ByteWidth = sizeof(ConstantBuffer);
	InitData.pSyMem = &cb;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pConstantBuffer);//�½���������������ʹ�ó�ʼ����
	g_pImmediateContext->SetConstantBuffer(1, 1, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;
	initdevicefinished = true;
	//������ͼ
	g_pImmediateContext->Clearnormaltexture();
	//g_pImmediateContext->normaltexturesource.readTGA("");
	g_pImmediateContext->texturesource.readTGA("1.1.tga");
	g_pImmediateContext->texturesource.LoadTexture(L"redlol.dds", 100,100);
	//����ģ���ļ�
	hr = g_pd3dDevice->CreateMesh(&g_mesh);
	if (FAILED(hr))
		return hr;
	ReadObjFile("lion.obj", g_mesh);
	g_pImmediateContext->SetMesh(g_mesh);
	return S_OK;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	UINT i, j;
	static int cxClient, cyClient;
	static float xPos = 0;
	static float yPos = 0;
	static float Xoffset = 0;
	static float Yoffset = 0;
	POINT p;
	POINT apt[1000];
	float controllevel = 0.05;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		SetCapture(g_hwnd);
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;
	case WM_MOUSEMOVE:
		GetCursorPos(&p);
		Xoffset = p.x - 512;
		Yoffset = 500 - p.y;
		xPos = p.x;
		yPos = p.y;
		*cb.At = VECTOR4(0, 1, 0, 0) + cb.cameraXaxis * (Xoffset * controllevel) + cb.cameraYaxis * (Yoffset * controllevel);
		resetConstantBuffer(&cb);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			*cb.eye = *cb.eye + VECTOR4(cb.cameraZaxis.x, -cb.cameraZaxis.y, cb.cameraZaxis.z, 0)* controllevel*2;
			*cb.At = *cb.At + cb.cameraZaxis * controllevel * 2;
			resetConstantBuffer(&cb);
			return 0;
		case VK_DOWN:
			*cb.eye = *cb.eye - VECTOR4(cb.cameraZaxis.x, -cb.cameraZaxis.y, cb.cameraZaxis.z, 0)* controllevel*2;
			*cb.At = *cb.At - cb.cameraZaxis * controllevel * 2;
			resetConstantBuffer(&cb);
			return 0;
		case VK_LEFT:
			*cb.eye = *cb.eye - cb.cameraXaxis * controllevel * 2;
			*cb.At = *cb.At - cb.cameraXaxis * controllevel * 2;
			resetConstantBuffer(&cb);
			return 0;
		case VK_RIGHT:
			*cb.eye = *cb.eye + cb.cameraXaxis * controllevel * 2;
			*cb.At = *cb.At + cb.cameraXaxis * controllevel * 2;
			resetConstantBuffer(&cb);
			return 0;
		case 0x57://w
			*cb.eye = *cb.eye - cb.cameraYaxis * 2;
			*cb.At = *cb.At - cb.cameraYaxis * 2;
			resetConstantBuffer(&cb);
			return 0;
		case 0x53://s
			*cb.eye = *cb.eye + cb.cameraYaxis * 2;
			*cb.At = *cb.At + cb.cameraYaxis * 2;
			resetConstantBuffer(&cb);
			return 0;
		case 0x55://u
			cb.lightPos.y -= 1;
			resetConstantBuffer(&cb);
			return 0;
		case 0x49://i
			cb.lightPos.y += 1;
			resetConstantBuffer(&cb);
			return 0;
		case 0x4A://j
			cb.lightPos.x += 1;
			resetConstantBuffer(&cb);
			return 0;
		case 0X4B://k
			cb.lightPos.x -= 1;
			resetConstantBuffer(&cb);
			return 0;
		case 0X4E://n
			cb.lightPos.z += 1;
			resetConstantBuffer(&cb);
			return 0;
		case 0X4D://m
			cb.lightPos.z -= 1;
			resetConstantBuffer(&cb);
			return 0;
		case 0x31:
			renderer.devicecontext->rendermodel = 1;
			return 0;
		case 0x32:
			renderer.devicecontext->rendermodel = 2;
			return 0;
		case 0x33:
			renderer.devicecontext->rendermodel = 3;
			return 0;
		}
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = LOWORD(lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
void CleanupDevice()
{
}
void Render(int indexnum)
{
	renderer.devicecontext->ClearRenderTargetView();
	renderer.devicecontext->ClearDepthView();
	renderer.devicecontext->DrawMesh();
	renderer.devicecontext->DrawLight();
	for (int x = 0; x < 1024; x++) {
		for (int y = 0; y < 768; y++) {
			buffer[int(y) * 1024 * 3 + (int(x) + 1) * 3 - 1] = renderer.swapchain->surface->data[x][y].r;//r
			buffer[int(y) * 1024 * 3 + (int(x) + 1) * 3 - 2] = renderer.swapchain->surface->data[x][y].g;//g
			buffer[int(y) * 1024 * 3 + (int(x) + 1) * 3 - 3] = renderer.swapchain->surface->data[x][y].b;//b
		}
	}
	SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, buffer, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
	renderer.swapchain->Present();
}
void resetConstantBuffer(ConstantBuffer* cb)
{
	VECTOR4 temp;
	cb->World = MatrixIdentity();
	cb->View = MatrixLookAtLH(cb->eye,cb->At,cb->Up);
	temp = *cb->At - *cb->eye;
	cb->cameraZaxis = Vec4Normalize(&temp);
	temp = Vec4Cross(cb->Up, &cb->cameraZaxis);
	cb->cameraXaxis = Vec4Normalize(&temp);
	cb->cameraYaxis = Vec4Cross(&cb->cameraZaxis, &cb->cameraXaxis);
	cb->Projection = MatrixPerspectiveLH(0.8, 1, 0.01, 500);
	cb->ka = 0.1;
	cb->kd = 1;
	cb->ks = 0;
	cb->a = 0;
	//cb->lightPos = VECTOR4(10, 5, 0, 1);
	cb->id = COLOR(0, 0, 200, 1);
	cb->is = COLOR(0, 0, 200, 1);
	cb->ia = COLOR(0, 0, 255, 1);
}
void DrawFPS(HDC hdc)
{
	static int t = 0;
	int fps = 0;
	int tmp = GetTickCount() - t;
	char str[50] = { 0 };
	if (tmp)
		fps = 1000 / tmp;
	t = GetTickCount();
	RECT rc = { 0,0,30,20 };
	if (fps)
		std::cout << " fps: " << fps << std::endl;
}