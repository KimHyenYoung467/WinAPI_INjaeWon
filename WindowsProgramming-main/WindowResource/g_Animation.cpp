#include <windows.h>
#include <stdio.h>
#include "g_Animation.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

//���� ���� 
LPCTSTR TitleName = TEXT("�͸��� Į���� ���� �ƽó���.");
LPCTSTR SzClassName = TEXT("�� �־ִ� �����Դϴ�."); 

//�ػ� ũ�� 
int GDIwidth = 1920; 
int GDIheight = 1080; 

// ������ ���̵� 
HWND hWnd; 
HDC FrontBufferDC;		  //���� ���� 
HDC BackBufferDC;		  //�ĸ� ���� 

HBITMAP BackBufferBitMap; // ��Ʈ�� �ĸ� ���� �ڵ� 

BITMAP bitmapInfo; //��Ʈ���� ����
ULONG_PTR GdiplusToken; // GDI ��ū 


// �ܼ� �ʱ�ȭ
void InitConsole()
{
	// ������ ��� �ܼ� �α׸� ���� �� �ֵ��� �Ѵ�. 
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"������ �޽��� �ܼ� �α�");
	printf("�ܼ� �α� ����...\n\n");
}
void UninitConsole()
{
	// ǥ�� ��� ��Ʈ�� �ݱ�
	fclose(stdout);
	// �ܼ� ����
	FreeConsole();
}

void RenderInitiallize()
{
	FrontBufferDC = GetDC(hWnd);
	BackBufferDC = CreateCompatibleDC(FrontBufferDC);
	BackBufferBitMap = CreateCompatibleBitmap(FrontBufferDC, GDIwidth, GDIheight);
	// ��Ʈ�ʰ� DC ���� 
	HBITMAP hPrevBitmap = (HBITMAP)SelectObject(BackBufferDC, BackBufferBitMap);
}


//GDI+ �ִϸ��̼� �ʱ�ȭ 
void  GDIplusAnimInit()
{
	//GDI+ ���̺귯���� �ʱ�ȭ�� �� ���Ǵ� ����ü(struct)
	Gdiplus::GdiplusStartupInput GSI; // GDI+ ������ �� ���� �ڵ鰪? 
	Gdiplus::GdiplusStartup(&GdiplusToken, &GSI, nullptr);
}

Gdiplus::Bitmap* GetResouseInfo(HBITMAP hPrevBitmap)
{
	// BackBufferDC�� �޾ƿͼ� �׷��� Ÿ�� �������� BackBufferGraphics �� �����Ѵ�. 
	Gdiplus::Graphics* BackBufferGraphics = Gdiplus::Graphics::FromHDC(BackBufferDC);

	bitmapInfo = { }; 
	
	// ���� ���� ���� ������ �ҷ����� 
	GetObject(hPrevBitmap, sizeof(BITMAP), &bitmapInfo);

	// �̹��� �ҷ��´�. 
	// ��� �ּҷ� ���ҽ� ��ġ�� �޾ƿ´�. 
	Gdiplus::Bitmap* ImageBitmapFile = new Gdiplus::Bitmap(L"..\WorkBMP\WorkBMP\pc_0_1_spr_0.png");
	::

	if (ImageBitmapFile->GetLastStatus() != Gdiplus::Ok)
	{
		MessageBox(hWnd, L"PNG ���� �ε� ����", L"����", MB_ICONERROR);
		PostQuitMessage(0);
	}
	return ImageBitmapFile;
}

void ImgRender(Gdiplus::Graphics* graphics, Gdiplus::Bitmap* bitmap)
{
	
	UINT BitWidth = bitmap->GetWidth();
	UINT BitHeight = bitmap->GetHeight();
	
	graphics->DrawImage(bitmap, 0, 0, BitWidth, BitHeight);

	BitBlt(FrontBufferDC, 0, 0, BitWidth, BitHeight, SRCCOPY);
}
void PrintLastErrorMsg()
{
	DWORD errorCode = GetLastError(); 
	LPVOID IpMsgBuf; // �޽��� ���� 

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �⺻ ���
		(LPSTR)&IpMsgBuf,
		0,
		NULL
	);
	if (IpMsgBuf)
	{
		printf("���� �ڵ�: %lu\n���� �޽���: %s\n", errorCode, (char*)IpMsgBuf);
		LocalFree(IpMsgBuf); // �Ҵ�� ���� ����
	}
	else
	{
		printf("���� �ڵ�: %lu (�޽����� ã�� �� ����)\n", errorCode);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	char szPath[MAX_PATH] = { 0, };

	// ���� �۾� ���丮�� �޾ƿ´�. 
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = SzClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// �⺻ Ŀ�� ���
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// �⺻ ������ ���
	RegisterClass(&wc);

	// ���ϴ� ũ�Ⱑ �����Ǿ� ����
	RECT rcClient = { 0, 0, (LONG)GDIwidth, (LONG)GDIheight };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//����
	HWND hwnd = CreateWindow(
		SzClassName,
		TitleName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

}