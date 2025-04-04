#include <windows.h>
#include <stdio.h>
#include "g_Animation.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

//전역 변수 
LPCTSTR TitleName = TEXT("귀멸의 칼날에 대해 아시나요.");
LPCTSTR SzClassName = TEXT("제 최애는 기유입니다."); 

//해상도 크기 
int GDIwidth = 1920; 
int GDIheight = 1080; 

// 윈도우 아이디 
HWND hWnd; 
HDC FrontBufferDC;		  //전면 버퍼 
HDC BackBufferDC;		  //후면 버퍼 

HBITMAP BackBufferBitMap; // 비트맵 후면 버퍼 핸들 

BITMAP bitmapInfo; //비트맵의 정보
ULONG_PTR GdiplusToken; // GDI 토큰 


// 콘솔 초기화
void InitConsole()
{
	// 파일을 열어서 콘솔 로그를 찍을 수 있도록 한다. 
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"윈도우 메시지 콘솔 로그");
	printf("콘솔 로그 시작...\n\n");
}
void UninitConsole()
{
	// 표준 출력 스트림 닫기
	fclose(stdout);
	// 콘솔 해제
	FreeConsole();
}

void RenderInitiallize()
{
	FrontBufferDC = GetDC(hWnd);
	BackBufferDC = CreateCompatibleDC(FrontBufferDC);
	BackBufferBitMap = CreateCompatibleBitmap(FrontBufferDC, GDIwidth, GDIheight);
	// 비트맵과 DC 연결 
	HBITMAP hPrevBitmap = (HBITMAP)SelectObject(BackBufferDC, BackBufferBitMap);
}


//GDI+ 애니메이션 초기화 
void  GDIplusAnimInit()
{
	//GDI+ 라이브러리를 초기화할 때 사용되는 구조체(struct)
	Gdiplus::GdiplusStartupInput GSI; // GDI+ 시작할 때 넣을 핸들값? 
	Gdiplus::GdiplusStartup(&GdiplusToken, &GSI, nullptr);
}

Gdiplus::Bitmap* GetResouseInfo(HBITMAP hPrevBitmap)
{
	// BackBufferDC를 받아와서 그래픽 타입 포인터인 BackBufferGraphics 에 저장한다. 
	Gdiplus::Graphics* BackBufferGraphics = Gdiplus::Graphics::FromHDC(BackBufferDC);

	bitmapInfo = { }; 
	
	// 가로 세로 길이 설정해 불러오기 
	GetObject(hPrevBitmap, sizeof(BITMAP), &bitmapInfo);

	// 이미지 불러온다. 
	// 상대 주소로 리소스 위치를 받아온다. 
	Gdiplus::Bitmap* ImageBitmapFile = new Gdiplus::Bitmap(L"..\WorkBMP\WorkBMP\pc_0_1_spr_0.png");
	::

	if (ImageBitmapFile->GetLastStatus() != Gdiplus::Ok)
	{
		MessageBox(hWnd, L"PNG 파일 로드 실패", L"오류", MB_ICONERROR);
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
	LPVOID IpMsgBuf; // 메시지 버퍼 

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 기본 언어
		(LPSTR)&IpMsgBuf,
		0,
		NULL
	);
	if (IpMsgBuf)
	{
		printf("오류 코드: %lu\n오류 메시지: %s\n", errorCode, (char*)IpMsgBuf);
		LocalFree(IpMsgBuf); // 할당된 버퍼 해제
	}
	else
	{
		printf("오류 코드: %lu (메시지를 찾을 수 없음)\n", errorCode);
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

	// 현재 작업 디렉토리를 받아온다. 
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = SzClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 기본 아이콘 모양
	RegisterClass(&wc);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, (LONG)GDIwidth, (LONG)GDIheight };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
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